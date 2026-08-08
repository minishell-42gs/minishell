#include "cmd.h"
#include "parsing_facade.h"
#include "unity.h"

static t_parsing_facade	g_test_facade;
static t_cmd_list		g_test_cmd_list;

/* 각 테스트 전에 parsing facade와 명령 리스트를 초기화한다. */
void	setUp(void)
{
	TEST_ASSERT_EQUAL_INT(OK, parsing_facade_init(&g_test_facade));
	TEST_ASSERT_EQUAL_INT(OK, cmd_list_init(&g_test_cmd_list));
}

/* 각 테스트가 끝난 뒤 생성된 명령과 facade 자원을 정리한다. */
void	tearDown(void)
{
	if (g_test_cmd_list.head != NULL)
		g_test_cmd_list.destroy(&g_test_cmd_list);
	g_test_facade.destroy(&g_test_facade);
}

/* ls -l | wc -l 파이프라인이 순서대로 연결된 두 명령이 되는지 확인한다. */
static void	assert_ls_pipe_wc_is_parsed(const char *line)
{
	char	*envp[] = {"PATH=/bin", NULL};
	t_cmd	*first;
	t_cmd	*second;

	TEST_ASSERT_EQUAL_INT(OK, parsing_facade_parse(&g_test_facade, line,
			&g_test_cmd_list, envp));
	first = g_test_cmd_list.head;
	TEST_ASSERT_NOT_NULL(first);
	second = first->next;
	TEST_ASSERT_NOT_NULL(second);
	TEST_ASSERT_EQUAL_STRING("ls", first->argv[0]);
	TEST_ASSERT_EQUAL_STRING("-l", first->argv[1]);
	TEST_ASSERT_NULL(first->argv[2]);
	TEST_ASSERT_EQUAL_STRING("wc", second->argv[0]);
	TEST_ASSERT_EQUAL_STRING("-l", second->argv[1]);
	TEST_ASSERT_NULL(second->argv[2]);
	TEST_ASSERT_NULL(second->next);
}

/* 문법 오류가 나면 명령 리스트를 만들지 않는지 확인한다. */
static void	assert_pipeline_parse_fails(const char *line)
{
	char	*envp[] = {"PATH=/bin", NULL};

	TEST_ASSERT_EQUAL_INT(FAIL, parsing_facade_parse(&g_test_facade, line,
			&g_test_cmd_list, envp));
	TEST_ASSERT_NULL(g_test_cmd_list.head);
}

/* facade가 ls -l 입력을 실행 가능한 단일 argv로 변환하는지 확인한다. */
void	test_facade_parses_ls_with_option_into_one_command(void)
{
	char	*envp[] = {"PATH=/bin", NULL};
	t_cmd	*cmd;

	TEST_ASSERT_EQUAL_INT(OK, parsing_facade_parse(&g_test_facade,
			"ls -l", &g_test_cmd_list, envp));
	cmd = g_test_cmd_list.head;
	TEST_ASSERT_NOT_NULL(cmd);
	TEST_ASSERT_EQUAL_STRING("ls", cmd->argv[0]);
	TEST_ASSERT_EQUAL_STRING("-l", cmd->argv[1]);
	TEST_ASSERT_NULL(cmd->argv[2]);
	TEST_ASSERT_NULL(cmd->redirs);
	TEST_ASSERT_NULL(cmd->next);
}

/* 공백으로 구분된 파이프라인을 두 명령으로 분리하는지 확인한다. */
void	test_facade_parses_a_pipeline_with_spaces(void)
{
	assert_ls_pipe_wc_is_parsed("ls -l | wc -l");
}

/* 파이프 양옆에 공백이 없어도 두 명령으로 분리하는지 확인한다. */
void	test_facade_parses_a_pipeline_without_spaces(void)
{
	assert_ls_pipe_wc_is_parsed("ls -l|wc -l");
}

/* 파이프 뒤에 명령이 없으면 문법 오류로 거부하는지 확인한다. */
void	test_facade_rejects_a_trailing_pipe(void)
{
	assert_pipeline_parse_fails("ls -l|");
}

/* 파이프 사이에 명령이 없으면 문법 오류로 거부하는지 확인한다. */
void	test_facade_rejects_consecutive_pipes(void)
{
	assert_pipeline_parse_fails("ls -l |  |");
}

/* 파이프로 시작하는 입력을 문법 오류로 거부하는지 확인한다. */
void	test_facade_rejects_a_leading_pipe(void)
{
	assert_pipeline_parse_fails("| ls -l");
}

/* 과제에서 지원하지 않는 연산자를 문법 오류로 거부하는지 확인한다. */
void	test_facade_rejects_unsupported_operators(void)
{
	assert_pipeline_parse_fails("ls -l || wc -l");
	assert_pipeline_parse_fails("ls -l ; wc -l");
	assert_pipeline_parse_fails("ls -l & wc -l");
	assert_pipeline_parse_fails("ls -l && wc -l");
}

/* facade가 NULL 입력을 거부하고 명령 리스트를 비워 두는지 확인한다. */
void	test_facade_rejects_null_line(void)
{
	TEST_ASSERT_EQUAL_INT(FAIL, parsing_facade_parse(&g_test_facade,
			NULL, &g_test_cmd_list, NULL));
	TEST_ASSERT_NULL(g_test_cmd_list.head);
}

/* Unity 테스트를 등록하고 실행한 뒤 테스트 결과를 반환한다. */
int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_facade_parses_ls_with_option_into_one_command);
	RUN_TEST(test_facade_parses_a_pipeline_with_spaces);
	RUN_TEST(test_facade_parses_a_pipeline_without_spaces);
	RUN_TEST(test_facade_rejects_a_trailing_pipe);
	RUN_TEST(test_facade_rejects_consecutive_pipes);
	RUN_TEST(test_facade_rejects_a_leading_pipe);
	RUN_TEST(test_facade_rejects_unsupported_operators);
	RUN_TEST(test_facade_rejects_null_line);
	return (UNITY_END());
}
