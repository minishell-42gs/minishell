#include "cmd.h"
#include "libft.h"
#include "parser.h"
#include "token.h"
#include "unity.h"
#include <stdlib.h>

static t_parser			g_test_parser;
static t_token_list		g_test_token_list;
static t_cmd_list		g_test_cmd_list;

/* 각 테스트 전에 parser와 입출력 리스트를 초기화한다. */
void	setUp(void)
{
	TEST_ASSERT_EQUAL_INT(OK, parser_init(&g_test_parser));
	TEST_ASSERT_EQUAL_INT(OK, token_list_init(&g_test_token_list));
	TEST_ASSERT_EQUAL_INT(OK, cmd_list_init(&g_test_cmd_list));
}

/* 각 테스트가 끝난 뒤 생성된 명령과 토큰을 정리한다. */
void	tearDown(void)
{
	if (g_test_cmd_list.head != NULL)
		g_test_cmd_list.destroy(&g_test_cmd_list);
	if (g_test_token_list.head != NULL)
		g_test_token_list.destroy(&g_test_token_list);
	g_test_parser.destroy(&g_test_parser);
}

/* parser 입력용 단어 토큰을 fixture 리스트에 등록한다. */
static void	append_word_token(const char *value)
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
}

/* parser가 ls와 -l 토큰을 하나의 명령 argv로 묶는지 확인한다. */
void	test_parser_builds_one_command_from_ls_and_option(void)
{
	t_cmd	*cmd;

	append_word_token("ls");
	append_word_token("-l");
	TEST_ASSERT_EQUAL_INT(OK, parser_run(&g_test_parser,
			g_test_token_list.head, &g_test_cmd_list));
	cmd = g_test_cmd_list.head;
	TEST_ASSERT_NOT_NULL(cmd);
	TEST_ASSERT_EQUAL_STRING("ls", cmd->argv[0]);
	TEST_ASSERT_EQUAL_STRING("-l", cmd->argv[1]);
	TEST_ASSERT_NULL(cmd->argv[2]);
	TEST_ASSERT_NULL(cmd->redirs);
	TEST_ASSERT_NULL(cmd->next);
}

/* parser가 NULL 입력을 거부하고 명령 리스트를 변경하지 않는지 확인한다. */
void	test_parser_rejects_null_arguments(void)
{
	append_word_token("ls");
	TEST_ASSERT_EQUAL_INT(FAIL,
		parser_run(&g_test_parser, NULL, &g_test_cmd_list));
	TEST_ASSERT_EQUAL_INT(FAIL,
		parser_run(&g_test_parser, g_test_token_list.head, NULL));
	TEST_ASSERT_NULL(g_test_cmd_list.head);
}

/* Unity 테스트를 등록하고 실행한 뒤 테스트 결과를 반환한다. */
int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_parser_builds_one_command_from_ls_and_option);
	RUN_TEST(test_parser_rejects_null_arguments);
	return (UNITY_END());
}
