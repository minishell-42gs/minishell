#include "lexer.h"
#include "token.h"
#include "unity.h"

static t_lexer		g_test_lexer;
static t_token_list	g_test_token_list;

/* 각 테스트 전에 lexer와 비어 있는 토큰 리스트를 초기화한다. */
void	setUp(void)
{
	TEST_ASSERT_EQUAL_INT(OK, lexer_init(&g_test_lexer));
	TEST_ASSERT_EQUAL_INT(OK, token_list_init(&g_test_token_list));
}

/* 각 테스트가 끝난 뒤 생성된 토큰과 lexer 자원을 정리한다. */
void	tearDown(void)
{
	if (g_test_token_list.head != NULL)
		g_test_token_list.destroy(&g_test_token_list);
	g_test_lexer.destroy(&g_test_lexer);
}

/* 입력 전체가 하나의 단어 토큰으로 보존되는지 확인한다. */
static void	assert_line_stays_one_word(const char *line)
{
	t_token	*token;

	TEST_ASSERT_EQUAL_INT(OK,
		lexer_run(&g_test_lexer, line, &g_test_token_list));
	token = g_test_token_list.head;
	TEST_ASSERT_NOT_NULL(token);
	TEST_ASSERT_EQUAL_INT(TOKEN_WORD, token->type);
	TEST_ASSERT_EQUAL_STRING(line, token->value);
	TEST_ASSERT_NULL(token->next);
	TEST_ASSERT_EQUAL_PTR(token, g_test_token_list.tail);
}

static t_token	*assert_token(t_token *token, t_token_type type,
		const char *value)
{
	TEST_ASSERT_NOT_NULL(token);
	TEST_ASSERT_EQUAL_INT(type, token->type);
	TEST_ASSERT_EQUAL_STRING(value, token->value);
	return (token->next);
}

/* lexer가 ls -l을 순서가 보존된 두 단어 토큰으로 만드는지 확인한다. */
void	test_lexer_tokenizes_ls_with_option(void)
{
	t_token	*first;
	t_token	*second;

	TEST_ASSERT_EQUAL_INT(OK,
		lexer_run(&g_test_lexer, "ls -l", &g_test_token_list));
	first = g_test_token_list.head;
	TEST_ASSERT_NOT_NULL(first);
	second = first->next;
	TEST_ASSERT_NOT_NULL(second);
	TEST_ASSERT_EQUAL_INT(TOKEN_WORD, first->type);
	TEST_ASSERT_EQUAL_STRING("ls", first->value);
	TEST_ASSERT_EQUAL_INT(TOKEN_WORD, second->type);
	TEST_ASSERT_EQUAL_STRING("-l", second->value);
	TEST_ASSERT_NULL(second->next);
}

/* lexer가 여러 공백을 명령 인자에 포함하지 않는지 확인한다. */
void	test_lexer_ignores_repeated_spaces_between_arguments(void)
{
	t_token	*first;
	t_token	*second;

	TEST_ASSERT_EQUAL_INT(OK,
		lexer_run(&g_test_lexer, "  ls   -l  ", &g_test_token_list));
	first = g_test_token_list.head;
	TEST_ASSERT_NOT_NULL(first);
	second = first->next;
	TEST_ASSERT_NOT_NULL(second);
	TEST_ASSERT_EQUAL_STRING("ls", first->value);
	TEST_ASSERT_EQUAL_STRING("-l", second->value);
	TEST_ASSERT_NULL(second->next);
}

/* lexer가 공백 없는 파이프 양옆의 단어를 각각 분리하는지 확인한다. */
void	test_lexer_splits_words_around_a_pipe_without_spaces(void)
{
	t_token	*first;
	t_token	*second;
	t_token	*third;

	TEST_ASSERT_EQUAL_INT(OK,
		lexer_run(&g_test_lexer, "ls|wc", &g_test_token_list));
	first = g_test_token_list.head;
	TEST_ASSERT_NOT_NULL(first);
	second = first->next;
	TEST_ASSERT_NOT_NULL(second);
	third = second->next;
	TEST_ASSERT_NOT_NULL(third);
	TEST_ASSERT_EQUAL_STRING("ls", first->value);
	TEST_ASSERT_EQUAL_INT(TOKEN_PIPE, second->type);
	TEST_ASSERT_EQUAL_STRING("wc", third->value);
	TEST_ASSERT_NULL(third->next);
	TEST_ASSERT_EQUAL_PTR(third, g_test_token_list.tail);
}

/* lexer가 이중 파이프를 포함한 문자열 전체를 하나의 단어로 유지하는지 확인한다. */
void	test_lexer_keeps_double_pipe_within_a_word(void)
{
	assert_line_stays_one_word("ls||wc");
}

/* 문자열 경계에 이중 파이프만 있어도 하나의 단어로 처리하는지 확인한다. */
void	test_lexer_handles_standalone_double_pipe(void)
{
	assert_line_stays_one_word("||");
}

/* 문자열 시작의 이중 파이프에서 범위를 벗어나지 않는지 확인한다. */
void	test_lexer_handles_double_pipe_at_start_of_word(void)
{
	assert_line_stays_one_word("||wc");
}

/* 문자열 끝의 이중 파이프에서 종료 문자 뒤를 읽지 않는지 확인한다. */
void	test_lexer_handles_double_pipe_at_end_of_word(void)
{
	assert_line_stays_one_word("ls||");
}

/* 연속된 이중 파이프도 하나의 문자열 덩어리로 유지하는지 확인한다. */
void	test_lexer_handles_repeated_double_pipes(void)
{
	assert_line_stays_one_word("||||");
}

/* 따옴표 안의 단일 파이프는 일반 단어의 일부로 유지해야 한다. */
void	test_lexer_keeps_pipe_inside_double_quotes(void)
{
	t_token	*token;

	TEST_ASSERT_EQUAL_INT(OK,
		lexer_run(&g_test_lexer, "echo \"a|b\"", &g_test_token_list));
	token = assert_token(g_test_token_list.head, TOKEN_WORD, "echo");
	token = assert_token(token, TOKEN_WORD, "\"a|b\"");
	TEST_ASSERT_NULL(token);
}

/* 따옴표 안의 공백은 단어 경계가 아니어야 한다. */
void	test_lexer_keeps_space_inside_double_quotes(void)
{
	t_token	*token;

	TEST_ASSERT_EQUAL_INT(OK,
		lexer_run(&g_test_lexer, "echo \"a b\"", &g_test_token_list));
	token = assert_token(g_test_token_list.head, TOKEN_WORD, "echo");
	token = assert_token(token, TOKEN_WORD, "\"a b\"");
	TEST_ASSERT_NULL(token);
}

/* 닫는 따옴표 뒤의 단일 파이프는 파이프 토큰으로 분리해야 한다. */
void	test_lexer_splits_pipe_after_quoted_word(void)
{
	t_token	*token;

	TEST_ASSERT_EQUAL_INT(OK,
		lexer_run(&g_test_lexer, "echo \"a\"|cat", &g_test_token_list));
	token = assert_token(g_test_token_list.head, TOKEN_WORD, "echo");
	token = assert_token(token, TOKEN_WORD, "\"a\"");
	token = assert_token(token, TOKEN_PIPE, "|");
	token = assert_token(token, TOKEN_WORD, "cat");
	TEST_ASSERT_NULL(token);
}

/* 이중 파이프는 지원 대상이 아니므로 단어로 유지해야 한다. */
void	test_lexer_keeps_double_pipe_after_a_command(void)
{
	t_token	*token;

	TEST_ASSERT_EQUAL_INT(OK,
		lexer_run(&g_test_lexer, "echo ||", &g_test_token_list));
	token = assert_token(g_test_token_list.head, TOKEN_WORD, "echo");
	token = assert_token(token, TOKEN_WORD, "||");
	TEST_ASSERT_NULL(token);
}

/* 빈 입력과 공백뿐인 입력에서 토큰을 생성하지 않는지 확인한다. */
void	test_lexer_handles_empty_lines(void)
{
	TEST_ASSERT_EQUAL_INT(OK,
		lexer_run(&g_test_lexer, "", &g_test_token_list));
	TEST_ASSERT_EQUAL_INT(OK,
		lexer_run(&g_test_lexer, "   ", &g_test_token_list));
	TEST_ASSERT_NULL(g_test_token_list.head);
	TEST_ASSERT_NULL(g_test_token_list.tail);
}

/* lexer가 NULL 입력을 거부하고 리스트를 변경하지 않는지 확인한다. */
void	test_lexer_rejects_null_arguments(void)
{
	TEST_ASSERT_EQUAL_INT(FAIL,
		lexer_run(&g_test_lexer, NULL, &g_test_token_list));
	TEST_ASSERT_EQUAL_INT(FAIL, lexer_run(&g_test_lexer, "ls", NULL));
	TEST_ASSERT_NULL(g_test_token_list.head);
}

/* Unity 테스트를 등록하고 실행한 뒤 테스트 결과를 반환한다. */
int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_lexer_tokenizes_ls_with_option);
	RUN_TEST(test_lexer_ignores_repeated_spaces_between_arguments);
	RUN_TEST(test_lexer_splits_words_around_a_pipe_without_spaces);
	RUN_TEST(test_lexer_keeps_double_pipe_within_a_word);
	RUN_TEST(test_lexer_handles_standalone_double_pipe);
	RUN_TEST(test_lexer_handles_double_pipe_at_start_of_word);
	RUN_TEST(test_lexer_handles_double_pipe_at_end_of_word);
	RUN_TEST(test_lexer_handles_repeated_double_pipes);
	RUN_TEST(test_lexer_keeps_pipe_inside_double_quotes);
	RUN_TEST(test_lexer_keeps_space_inside_double_quotes);
	RUN_TEST(test_lexer_splits_pipe_after_quoted_word);
	RUN_TEST(test_lexer_keeps_double_pipe_after_a_command);
	RUN_TEST(test_lexer_handles_empty_lines);
	RUN_TEST(test_lexer_rejects_null_arguments);
	return (UNITY_END());
}
