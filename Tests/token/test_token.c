#include "libft.h"
#include "token.h"
#include "unity.h"
#include <stdlib.h>

static t_token_factory	g_factory;
static t_token_list		g_tokens;
static t_token			g_stack_token;
static int				g_stack_token_initialized;

void	setUp(void)
{
	g_stack_token_initialized = 0;
	TEST_ASSERT_EQUAL_INT(OK, token_factory_init(&g_factory));
	TEST_ASSERT_EQUAL_INT(OK, token_list_init(&g_tokens));
}

void	tearDown(void)
{
	if (g_stack_token_initialized)
		g_stack_token.destroy(&g_stack_token);
	if (g_tokens.head != NULL)
		g_tokens.destroy(&g_tokens);
	g_factory.destroy(&g_factory);
}

static t_token	*create_tracked_token(const char *value)
{
	t_token		*token;
	t_status	status;

	token = token_factory_create(&g_factory, value);
	TEST_ASSERT_NOT_NULL_MESSAGE(token, "token allocation failed");
	status = token_list_add_token(&g_tokens, token);
	if (status != OK)
	{
		token->destroy(token);
		free(token);
	}
	TEST_ASSERT_EQUAL_INT(OK, status);
	return (token);
}

void	test_token_init_stores_its_fields_and_starts_unlinked(void)
{
	char		*value;
	t_status	status;

	value = ft_strdup("echo");
	TEST_ASSERT_NOT_NULL(value);
	g_stack_token.next = &g_stack_token;
	status = token_init(&g_stack_token, TOKEN_WORD, value);
	if (status == OK)
		g_stack_token_initialized = 1;
	else
		free(value);
	TEST_ASSERT_EQUAL_INT(OK, status);
	TEST_ASSERT_EQUAL_INT(TOKEN_WORD, g_stack_token.type);
	TEST_ASSERT_EQUAL_PTR(value, g_stack_token.value);
	TEST_ASSERT_NULL(g_stack_token.next);
	TEST_ASSERT_NOT_NULL(g_stack_token.destroy);
}

void	test_factory_creates_a_word_with_its_own_value_copy(void)
{
	char	input[] = "echo";
	t_token	*token;

	token = create_tracked_token(input);
	TEST_ASSERT_EQUAL_INT(TOKEN_WORD, token->type);
	TEST_ASSERT_EQUAL_STRING("echo", token->value);
	TEST_ASSERT_NOT_EQUAL(input, token->value);
	input[0] = 'x';
	TEST_ASSERT_EQUAL_STRING("echo", token->value);
}

void	test_factory_classifies_a_single_pipe_as_pipe(void)
{
	t_token	*token;

	token = create_tracked_token("|");
	TEST_ASSERT_EQUAL_INT(TOKEN_PIPE, token->type);
	TEST_ASSERT_EQUAL_STRING("|", token->value);
}

void	test_factory_does_not_classify_double_pipe_as_single_pipe(void)
{
	t_token	*token;

	token = create_tracked_token("||");
	TEST_ASSERT_EQUAL_INT(TOKEN_WORD, token->type);
}

void	test_new_token_list_is_empty(void)
{
	TEST_ASSERT_NULL(g_tokens.head);
	TEST_ASSERT_NOT_NULL(g_tokens.destroy);
}

void	test_list_appends_tokens_in_insertion_order(void)
{
	t_token	*first;
	t_token	*second;

	first = create_tracked_token("echo");
	second = create_tracked_token("|");
	TEST_ASSERT_EQUAL_PTR(first, g_tokens.head);
	TEST_ASSERT_EQUAL_PTR(second, first->next);
	TEST_ASSERT_NULL(second->next);
}

void	test_list_rejects_null_arguments_without_changing_the_list(void)
{
	char		*value;
	t_status	status;

	value = ft_strdup("echo");
	TEST_ASSERT_NOT_NULL(value);
	status = token_init(&g_stack_token, TOKEN_WORD, value);
	if (status == OK)
		g_stack_token_initialized = 1;
	else
		free(value);
	TEST_ASSERT_EQUAL_INT(OK, status);
	TEST_ASSERT_EQUAL_INT(FAIL, token_list_add_token(NULL, &g_stack_token));
	TEST_ASSERT_EQUAL_INT(FAIL, token_list_add_token(&g_tokens, NULL));
	TEST_ASSERT_NULL(g_tokens.head);
}

int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_token_init_stores_its_fields_and_starts_unlinked);
	RUN_TEST(test_factory_creates_a_word_with_its_own_value_copy);
	RUN_TEST(test_factory_classifies_a_single_pipe_as_pipe);
	RUN_TEST(test_factory_does_not_classify_double_pipe_as_single_pipe);
	RUN_TEST(test_new_token_list_is_empty);
	RUN_TEST(test_list_appends_tokens_in_insertion_order);
	RUN_TEST(test_list_rejects_null_arguments_without_changing_the_list);
	return (UNITY_END());
}
