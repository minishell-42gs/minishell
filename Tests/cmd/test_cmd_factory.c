#include "cmd.h"
#include "libft.h"
#include "token.h"
#include "unity.h"
#include <stdlib.h>

static t_cmd_factory	g_test_cmd_factory;
static t_token_list		g_test_token_list;
static t_cmd				*g_test_cmd;

/* 각 테스트 전에 팩토리와 토큰 리스트를 초기화한다. */
void	setUp(void)
{
	g_test_cmd = NULL;
	TEST_ASSERT_EQUAL_INT(OK, cmd_factory_init(&g_test_cmd_factory));
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
	g_test_cmd_factory.destroy(&g_test_cmd_factory);
}

/* 명령 factory 테스트용 단어 토큰을 fixture 리스트에 등록한다. */
static t_token	*append_word_token(const char *value)
{
	t_token		*token;
	char		*value_copy;
	t_status	status;

	value_copy = ft_strdup(value);
	TEST_ASSERT_NOT_NULL_MESSAGE(value_copy, "value allocation failed");
	token = ft_calloc(1, sizeof(t_token));
	if (token == NULL)
		free(value_copy);
	TEST_ASSERT_NOT_NULL_MESSAGE(token, "token allocation failed");
	status = token_init(token, TOKEN_WORD, value_copy);
	if (status != OK)
	{
		free(token);
		free(value_copy);
	}
	TEST_ASSERT_EQUAL_INT(OK, status);
	status = token_list_add_token(&g_test_token_list, token);
	if (status != OK)
	{
		token->destroy(token);
		free(token);
	}
	TEST_ASSERT_EQUAL_INT(OK, status);
	return (token);
}

/* 팩토리가 ls와 -l을 하나의 명령 argv로 만드는지 확인한다. */
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
