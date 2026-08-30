#include "cmd.h"
#include "env.h"
#include "executor.h"
#include "parsing_facade.h"
#include "unity.h"
#include <stdio.h>

static t_parsing_facade	g_facade;
static t_env_list		g_env_list;
static t_executor		g_executor;
static t_cmd_list		g_cmd_list;

/* 각 테스트 전에 facade, env, executor, 명령 리스트를 초기화한다. */
void	setUp(void)
{
	char	*envp[] = {"PATH=/bin:/usr/bin", NULL};

	TEST_ASSERT_EQUAL_INT(OK, parsing_facade_init(&g_facade));
	TEST_ASSERT_EQUAL_INT(OK, env_list_init(&g_env_list, envp));
	TEST_ASSERT_EQUAL_INT(OK, executor_init(&g_executor, &g_env_list));
	TEST_ASSERT_EQUAL_INT(OK, cmd_list_init(&g_cmd_list));
}

/* 각 테스트가 끝난 뒤 생성한 자원을 init의 역순으로 정리한다. */
void	tearDown(void)
{
	if (g_cmd_list.head != NULL)
		g_cmd_list.destroy(&g_cmd_list);
	g_executor.destroy(&g_executor);
	g_env_list.destroy(&g_env_list);
	g_facade.destroy(&g_facade);
}

/* 한 줄을 파싱해 실행하고 executor_run의 반환값을 돌려준다.
 * fork 전에 stdout을 비워야 자식이 Unity 출력 버퍼를 복제해 두 번 찍지 않는다. */
static t_status	run_line(const char *line)
{
	char	*envp[] = {"PATH=/bin:/usr/bin", NULL};

	TEST_ASSERT_EQUAL_INT(OK, parsing_facade_parse(&g_facade, line,
			&g_cmd_list, envp));
	fflush(stdout);
	return (executor_run(&g_executor, &g_cmd_list));
}

/* 생성 직후 executor가 env_list를 참조하고 종료 코드가 0인지 확인한다. */
void	test_executor_init_sets_reference_and_zero_status(void)
{
	TEST_ASSERT_EQUAL_PTR(&g_env_list, g_executor.env_list);
	TEST_ASSERT_EQUAL_INT(0, g_executor.last_status);
	TEST_ASSERT_NOT_NULL(g_executor.destroy);
}

/* 비어 있는 명령 리스트는 아무것도 실행하지 않고 OK를 반환해야 한다. */
void	test_executor_runs_empty_list_as_noop(void)
{
	TEST_ASSERT_EQUAL_INT(OK, executor_run(&g_executor, &g_cmd_list));
	TEST_ASSERT_EQUAL_INT(0, g_executor.last_status);
}

/* 정상 종료한 명령의 종료 코드 0이 last_status로 전달되는지 확인한다. */
void	test_executor_reports_zero_for_true(void)
{
	TEST_ASSERT_EQUAL_INT(OK, run_line("/bin/true"));
	TEST_ASSERT_EQUAL_INT(0, g_executor.last_status);
}

/* 프로그램이 정한 0이 아닌 종료 코드가 그대로 전달되는지 확인한다. */
void	test_executor_reports_one_for_false(void)
{
	TEST_ASSERT_EQUAL_INT(OK, run_line("/bin/false"));
	TEST_ASSERT_EQUAL_INT(1, g_executor.last_status);
}

/* PATH 탐색으로 ls를 찾아 옵션과 함께 실행하는지 확인한다 (목표 케이스). */
void	test_executor_runs_ls_with_option_via_path(void)
{
	TEST_ASSERT_EQUAL_INT(OK, run_line("ls -a"));
	TEST_ASSERT_EQUAL_INT(0, g_executor.last_status);
}

/* 없는 명령은 셸이 계속 돌 수 있도록 OK를 반환하되 종료 코드는 127이어야 한다. */
void	test_executor_reports_127_for_unknown_command(void)
{
	TEST_ASSERT_EQUAL_INT(OK, run_line("no_such_cmd_xyz"));
	TEST_ASSERT_EQUAL_INT(127, g_executor.last_status);
}

/* 연속 실행 시 last_status가 마지막 명령의 코드로 덮어써지는지 확인한다. */
void	test_executor_overwrites_status_with_latest_command(void)
{
	TEST_ASSERT_EQUAL_INT(OK, run_line("/bin/false"));
	TEST_ASSERT_EQUAL_INT(1, g_executor.last_status);
	g_cmd_list.destroy(&g_cmd_list);
	TEST_ASSERT_EQUAL_INT(OK, cmd_list_init(&g_cmd_list));
	TEST_ASSERT_EQUAL_INT(OK, run_line("/bin/true"));
	TEST_ASSERT_EQUAL_INT(0, g_executor.last_status);
}

/* Unity 테스트를 등록하고 실행한 뒤 테스트 결과를 반환한다. */
int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_executor_init_sets_reference_and_zero_status);
	RUN_TEST(test_executor_runs_empty_list_as_noop);
	RUN_TEST(test_executor_reports_zero_for_true);
	RUN_TEST(test_executor_reports_one_for_false);
	RUN_TEST(test_executor_runs_ls_with_option_via_path);
	RUN_TEST(test_executor_reports_127_for_unknown_command);
	RUN_TEST(test_executor_overwrites_status_with_latest_command);
	return (UNITY_END());
}
