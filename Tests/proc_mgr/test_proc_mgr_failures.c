#include "executor.h"
#include "parsing_facade.h"
#include "unity.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static t_executor executor;
static t_env_list env;
static t_parsing_facade facade;
static t_cmd_list commands;
static int fork_fail_at;
static int pipe_fail_at;
static int interrupt_wait;
static int fail_dup;
static int fork_calls;
static int pipe_calls;
static int child_count;
static pid_t children[8];

pid_t __real_fork(void);
int __real_pipe(int fds[2]);
pid_t __real_waitpid(pid_t pid, int *status, int options);
int __real_dup2(int oldfd, int newfd);

pid_t __wrap_fork(void)
{
	pid_t pid;

	if (++fork_calls == fork_fail_at)
		return (errno = EAGAIN, -1);
	pid = __real_fork();
	if (pid > 0 && child_count < 8)
		children[child_count++] = pid;
	return (pid);
}

int __wrap_pipe(int fds[2])
{
	if (++pipe_calls == pipe_fail_at)
		return (errno = EMFILE, -1);
	return (__real_pipe(fds));
}

pid_t __wrap_waitpid(pid_t pid, int *status, int options)
{
	if (interrupt_wait > 0)
	{
		interrupt_wait--;
		return (errno = EINTR, -1);
	}
	return (__real_waitpid(pid, status, options));
}

int __wrap_dup2(int oldfd, int newfd)
{
	if (fail_dup)
		return (errno = EBADF, -1);
	return (__real_dup2(oldfd, newfd));
}

void setUp(void)
{
	char *envp[] = {"PATH=/bin:/usr/bin", NULL};

	fork_fail_at = 0;
	pipe_fail_at = 0;
	interrupt_wait = 0;
	fail_dup = 0;
	fork_calls = 0;
	pipe_calls = 0;
	child_count = 0;
	TEST_ASSERT_EQUAL_INT(OK, env_list_init(&env, envp));
	TEST_ASSERT_EQUAL_INT(OK, parsing_facade_init(&facade, &env));
	TEST_ASSERT_EQUAL_INT(OK, executor_init(&executor, &env));
	TEST_ASSERT_EQUAL_INT(OK, cmd_list_init(&commands));
	alarm(5);
}

void tearDown(void)
{
	int i;

	alarm(0);
	for (i = 0; i < child_count; i++)
	{
		if (__real_waitpid(children[i], NULL, WNOHANG) == 0)
		{
			kill(children[i], SIGKILL);
			__real_waitpid(children[i], NULL, 0);
		}
	}
	commands.destroy(&commands);
	executor.destroy(&executor);
	facade.destroy(&facade);
	env.destroy(&env);
}

static t_status run_line(const char *line, int *status)
{
	t_parse_outcome outcome;

	outcome = parsing_facade_parse(&facade, line, &commands);
	TEST_ASSERT_EQUAL_INT(PARSE_OK, outcome.result);
	fflush(NULL);
	return (executor.run(&executor, &commands, status));
}

static int fd_count(void)
{
	int count = 0;
	int fd;

	for (fd = 0; fd < 256; fd++)
		if (fcntl(fd, F_GETFD) != -1)
			count++;
	return (count);
}

static void assert_children_reaped(void)
{
	int i;

	for (i = 0; i < child_count; i++)
	{
		errno = 0;
		TEST_ASSERT_EQUAL_INT(-1, __real_waitpid(children[i], NULL, WNOHANG));
		TEST_ASSERT_EQUAL_INT(ECHILD, errno);
	}
}

void test_interrupted_wait_retries_and_reaps_every_child(void)
{
	int status = 42;

	interrupt_wait = 3;
	TEST_ASSERT_EQUAL_INT(OK, run_line("/bin/true | /bin/false", &status));
	TEST_ASSERT_EQUAL_INT(1, status);
	TEST_ASSERT_EQUAL_INT(0, interrupt_wait);
	assert_children_reaped();
}

void test_partial_fork_failure_stops_blocking_children(void)
{
	int status = 42;
	int before = fd_count();

	fork_fail_at = 3;
	TEST_ASSERT_EQUAL_INT(FAIL,
		run_line("/bin/sleep 30 | /bin/cat | /bin/cat", &status));
	TEST_ASSERT_EQUAL_INT(42, status);
	TEST_ASSERT_EQUAL_INT(2, child_count);
	TEST_ASSERT_EQUAL_INT(before, fd_count());
	assert_children_reaped();
}

void test_first_fork_failure_closes_every_pipe(void)
{
	int status = 42;
	int before = fd_count();

	fork_fail_at = 1;
	TEST_ASSERT_EQUAL_INT(FAIL, run_line("/bin/true | /bin/cat", &status));
	TEST_ASSERT_EQUAL_INT(42, status);
	TEST_ASSERT_EQUAL_INT(0, child_count);
	TEST_ASSERT_EQUAL_INT(before, fd_count());
}

void test_partial_pipe_failure_releases_open_descriptors(void)
{
	int status = 42;
	int before = fd_count();

	pipe_fail_at = 2;
	TEST_ASSERT_EQUAL_INT(FAIL,
		run_line("/bin/true | /bin/cat | /bin/cat", &status));
	TEST_ASSERT_EQUAL_INT(42, status);
	TEST_ASSERT_EQUAL_INT(0, child_count);
	TEST_ASSERT_EQUAL_INT(before, fd_count());
}

void test_dup_failure_returns_child_failure_and_reaps_pipeline(void)
{
	int status = 42;
	int before = fd_count();

	fail_dup = 1;
	TEST_ASSERT_EQUAL_INT(OK, run_line("/bin/true | /bin/cat", &status));
	TEST_ASSERT_EQUAL_INT(1, status);
	TEST_ASSERT_EQUAL_INT(before, fd_count());
	assert_children_reaped();
}

void test_last_child_signal_sets_pipeline_status(void)
{
	int status = 42;
	t_cmd *last;
	t_parse_outcome outcome;

	outcome = parsing_facade_parse(&facade, "/bin/true | /bin/sh -c script",
		&commands);
	TEST_ASSERT_EQUAL_INT(PARSE_OK, outcome.result);
	last = commands.head->next;
	free(last->argv[2]);
	last->argv[2] = strdup("kill -TERM $$");
	TEST_ASSERT_NOT_NULL(last->argv[2]);
	fflush(NULL);
	TEST_ASSERT_EQUAL_INT(OK, executor.run(&executor, &commands, &status));
	TEST_ASSERT_EQUAL_INT(128 + SIGTERM, status);
	assert_children_reaped();
}

void test_repeated_pipelines_do_not_leak_parent_descriptors(void)
{
	int before = fd_count();
	int status = 42;
	int i;
	t_parse_outcome outcome;

	outcome = parsing_facade_parse(&facade, "/bin/true | /bin/cat", &commands);
	TEST_ASSERT_EQUAL_INT(PARSE_OK, outcome.result);
	for (i = 0; i < 12; i++)
	{
		child_count = 0;
		fflush(NULL);
		TEST_ASSERT_EQUAL_INT(OK, executor.run(&executor, &commands, &status));
		TEST_ASSERT_EQUAL_INT(0, status);
		TEST_ASSERT_EQUAL_INT(before, fd_count());
		assert_children_reaped();
	}
}

void test_child_restores_ignored_sigint(void)
{
	int status = 42;
	t_cmd *last;
	t_status result;
	void (*previous)(int);
	t_parse_outcome outcome;

	outcome = parsing_facade_parse(&facade, "/bin/true | /bin/sh -c script",
		&commands);
	TEST_ASSERT_EQUAL_INT(PARSE_OK, outcome.result);
	last = commands.head->next;
	free(last->argv[2]);
	last->argv[2] = strdup("kill -INT $$");
	TEST_ASSERT_NOT_NULL(last->argv[2]);
	fflush(NULL);
	previous = signal(SIGINT, SIG_IGN);
	result = executor.run(&executor, &commands, &status);
	signal(SIGINT, previous);
	TEST_ASSERT_EQUAL_INT(OK, result);
	TEST_ASSERT_EQUAL_INT(128 + SIGINT, status);
	assert_children_reaped();
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_interrupted_wait_retries_and_reaps_every_child);
	RUN_TEST(test_partial_fork_failure_stops_blocking_children);
	RUN_TEST(test_first_fork_failure_closes_every_pipe);
	RUN_TEST(test_partial_pipe_failure_releases_open_descriptors);
	RUN_TEST(test_dup_failure_returns_child_failure_and_reaps_pipeline);
	RUN_TEST(test_last_child_signal_sets_pipeline_status);
	RUN_TEST(test_repeated_pipelines_do_not_leak_parent_descriptors);
	RUN_TEST(test_child_restores_ignored_sigint);
	return (UNITY_END());
}
