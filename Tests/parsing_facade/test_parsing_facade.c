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

/* facade가 "ls -l" 입력을 lexer와 parser를 거쳐 하나의 argv로 변환하는지 확인한다.
 * facade에서 단계 연결이 끊기면 토큰은 만들어져도 명령이 생성되지 않거나,
 * argv 순서가 바뀌어 최종 실행 결과가 원래 입력과 달라진다.
 */
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

/* facade가 NULL 입력을 거부하고 명령 리스트를 비워 두는지 확인한다.
 * NULL line을 lexer로 넘기면 크래시할 수 있고, 실패 전에 명령을 추가하면
 * 잘못된 입력이 성공한 것처럼 남아 후속 실행의 원인이 된다.
 */
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
	RUN_TEST(test_facade_rejects_null_line);
	return (UNITY_END());
}
