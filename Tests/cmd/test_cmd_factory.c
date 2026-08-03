#include "cmd.h"
#include "token.h"
#include "unity.h"
#include <stdlib.h>

static t_cmd_factory	g_test_cmd_factory;
static t_token_factory	g_test_token_factory;
static t_token_list		g_test_token_list;
static t_cmd				*g_test_cmd;

/* 각 테스트 전에 팩토리와 토큰 리스트를 초기화한다. */
void	setUp(void)
{
	g_test_cmd = NULL;
	TEST_ASSERT_EQUAL_INT(OK, cmd_factory_init(&g_test_cmd_factory));
	TEST_ASSERT_EQUAL_INT(OK, token_factory_init(&g_test_token_factory));
	TEST_ASSERT_EQUAL_INT(OK, token_list_init(&g_test_token_list));
}

/* 각 테스트가 끝난 뒤 생성된 명령과 토큰을 정리한다. */
void	tearDown(void)
{
	if (g_test_cmd != NULL)
	{
		g_test_cmd->destroy(g_test_cmd);
		free(g_test_cmd);
	}
	if (g_test_token_list.head != NULL)
		g_test_token_list.destroy(&g_test_token_list);
	g_test_token_factory.destroy(&g_test_token_factory);
	g_test_cmd_factory.destroy(&g_test_cmd_factory);
}

/* 단어 토큰을 생성해 테스트용 토큰 리스트에 등록한다. */
static t_token	*append_word_token(const char *value)
{
	t_token		*token;
	t_status	status;

	token = token_factory_create(&g_test_token_factory, value);
	TEST_ASSERT_NOT_NULL_MESSAGE(token, "token allocation failed");
	status = token_list_add_token(&g_test_token_list, token);
	if (status != OK)
	{
		token->destroy(token);
		free(token);
	}
	TEST_ASSERT_EQUAL_INT(OK, status);
	return (token);
}

/* "ls"와 "-l" 단어 토큰을 하나의 명령 argv로 만드는지 확인한다.
 * 토큰마다 명령을 만들거나 토큰의 문자열 주소를 그대로 재사용하면
 * argv 순서가 깨지거나 토큰 리스트가 해제된 뒤 명령이 dangling pointer를 갖는다.
 */
void	test_factory_builds_single_command_argv_from_word_tokens(void)
{
	t_token	*cursor;
	t_token	*first;

	first = append_word_token("ls");
	append_word_token("-l");
	cursor = g_test_token_list.head;
	g_test_cmd = cmd_factory_create(&g_test_cmd_factory, &cursor);
	TEST_ASSERT_NOT_NULL(g_test_cmd);
	TEST_ASSERT_EQUAL_STRING("ls", g_test_cmd->argv[0]);
	TEST_ASSERT_EQUAL_STRING("-l", g_test_cmd->argv[1]);
	TEST_ASSERT_NULL(g_test_cmd->argv[2]);
	TEST_ASSERT_NOT_EQUAL_PTR(first->value, g_test_cmd->argv[0]);
	TEST_ASSERT_NULL(g_test_cmd->redirs);
	TEST_ASSERT_NULL(g_test_cmd->next);
	TEST_ASSERT_NULL(cursor);
}

/* Unity 테스트를 등록하고 실행한 뒤 테스트 결과를 반환한다. */
int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_factory_builds_single_command_argv_from_word_tokens);
	return (UNITY_END());
}
