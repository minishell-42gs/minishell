#include "error.h"
#include "unity.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define OUTPUT_SIZE 256

static char	g_output[OUTPUT_SIZE];
static int	g_status;

void	setUp(void)
{
	g_status = -1;
	g_output[0] = '\0';
}

void	tearDown(void)
{
}

static void	capture_error(const t_error_req *req)
{
	int		pipefd[2];
	int		saved_stderr;
	ssize_t	read_count;

	TEST_ASSERT_EQUAL_INT(0, pipe(pipefd));
	saved_stderr = dup(STDERR_FILENO);
	TEST_ASSERT_NOT_EQUAL(-1, saved_stderr);
	TEST_ASSERT_EQUAL_INT(STDERR_FILENO, dup2(pipefd[1], STDERR_FILENO));
	close(pipefd[1]);
	error_report(&g_status, req);
	TEST_ASSERT_EQUAL_INT(STDERR_FILENO, dup2(saved_stderr, STDERR_FILENO));
	close(saved_stderr);
	read_count = read(pipefd[0], g_output, OUTPUT_SIZE - 1);
	close(pipefd[0]);
	TEST_ASSERT_TRUE(read_count >= 0);
	g_output[read_count] = '\0';
}

void	test_error_report_prints_syntax_error_and_sets_status(void)
{
	t_error_req	req;

	req = (t_error_req){ERR_SYNTAX, 2, {.s_syntax = {"newline"}}};
	capture_error(&req);
	TEST_ASSERT_EQUAL_INT(2, g_status);
	TEST_ASSERT_EQUAL_STRING(
		"minishell: syntax error near unexpected token `newline'\n", g_output);
}

void	test_error_report_prints_command_not_found_and_sets_status(void)
{
	t_error_req	req;

	req = (t_error_req){ERR_CMD_NOT_FOUND, 127,
		{.s_cmd_not_found = {"foo"}}};
	capture_error(&req);
	TEST_ASSERT_EQUAL_INT(127, g_status);
	TEST_ASSERT_EQUAL_STRING("minishell: foo: command not found\n", g_output);
}

void	test_error_report_prints_execve_error_and_sets_status(void)
{
	t_error_req	req;
	char		expected[OUTPUT_SIZE];

	req = (t_error_req){ERR_ERRNO, 126, {.s_sys = {"./program", EACCES}}};
	snprintf(expected, OUTPUT_SIZE, "minishell: ./program: %s\n",
		strerror(EACCES));
	capture_error(&req);
	TEST_ASSERT_EQUAL_INT(126, g_status);
	TEST_ASSERT_EQUAL_STRING(expected, g_output);
}

void	test_error_report_prints_builtin_error_and_sets_status(void)
{
	t_error_req	req;

	req = (t_error_req){ERR_BUILTIN, 1,
		{.s_builtin = {"cd", "/none: No such file or directory"}}};
	capture_error(&req);
	TEST_ASSERT_EQUAL_INT(1, g_status);
	TEST_ASSERT_EQUAL_STRING("minishell: cd: /none: No such file or directory\n",
		g_output);
}

void	test_error_report_prints_redirection_error_and_sets_status(void)
{
	t_error_req	req;
	char		expected[OUTPUT_SIZE];

	req = (t_error_req){ERR_ERRNO, 1, {.s_sys = {"/protected/a", EACCES}}};
	snprintf(expected, OUTPUT_SIZE, "minishell: /protected/a: %s\n",
		strerror(EACCES));
	capture_error(&req);
	TEST_ASSERT_EQUAL_INT(1, g_status);
	TEST_ASSERT_EQUAL_STRING(expected, g_output);
}

void	test_error_report_prints_heredoc_eof_warning(void)
{
	t_error_req	req;

	req = (t_error_req){ERR_HEREDOC_EOF, 0, {.s_heredoc_eof = {"EOF"}}};
	capture_error(&req);
	TEST_ASSERT_EQUAL_INT(0, g_status);
	TEST_ASSERT_EQUAL_STRING("minishell: warning: here-document "
		"delimited by end-of-file (wanted `EOF')\n", g_output);
}

int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_error_report_prints_syntax_error_and_sets_status);
	RUN_TEST(test_error_report_prints_command_not_found_and_sets_status);
	RUN_TEST(test_error_report_prints_execve_error_and_sets_status);
	RUN_TEST(test_error_report_prints_builtin_error_and_sets_status);
	RUN_TEST(test_error_report_prints_redirection_error_and_sets_status);
	RUN_TEST(test_error_report_prints_heredoc_eof_warning);
	return (UNITY_END());
}
