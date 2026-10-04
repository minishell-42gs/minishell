#include "libft.h"
#include "token.h"
#include "unity.h"
#include <stdlib.h>

static t_token_list	g_test_token_list;
static t_token			g_test_token;
static int				g_test_token_initialized;

/* 각 테스트 전에 토큰 리스트 fixture를 초기화한다. */
void	setUp(void)
{
	g_test_token_initialized = 0;
	TEST_ASSERT_EQUAL_INT(OK, token_list_init(&g_test_token_list));
}

/* 각 테스트가 끝난 뒤 생성된 토큰과 테스트 자원을 정리한다. */
void	tearDown(void)
{
	if (g_test_token_initialized)
		g_test_token.destroy(&g_test_token);
	if (g_test_token_list.head != NULL)
		g_test_token_list.destroy(&g_test_token_list);
}

/* 리스트 테스트용 heap 토큰을 만들고 fixture 리스트에 등록한다. */
static t_token	*create_tracked_token(t_token_type type, const char *value)
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
	status = token_init(token, type, value_copy);
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

/* 토큰의 필드가 저장되고 연결되지 않은 상태로 초기화되는지 확인한다. */
void	test_token_init_stores_its_fields_and_starts_unlinked(void)
{
	char		*value;
	t_status	status;

	value = ft_strdup("echo");
	TEST_ASSERT_NOT_NULL(value);
	g_test_token.next = &g_test_token;
	status = token_init(&g_test_token, TOKEN_WORD, value);
	if (status == OK)
		g_test_token_initialized = 1;
	else
		free(value);
	TEST_ASSERT_EQUAL_INT(OK, status);
	TEST_ASSERT_EQUAL_INT(TOKEN_WORD, g_test_token.type);
	TEST_ASSERT_EQUAL_PTR(value, g_test_token.value);
	TEST_ASSERT_NULL(g_test_token.next);
	TEST_ASSERT_NOT_NULL(g_test_token.destroy);
}

/* 새 토큰 리스트가 비어 있고 소멸 함수를 제공하는지 확인한다. */
void	test_new_token_list_is_empty(void)
{
	TEST_ASSERT_NULL(g_test_token_list.head);
	TEST_ASSERT_NULL(g_test_token_list.tail);
	TEST_ASSERT_NOT_NULL(g_test_token_list.destroy);
}

/* 토큰 리스트가 삽입 순서대로 토큰을 연결하는지 확인한다. */
void	test_list_appends_tokens_in_insertion_order(void)
{
	t_token	*first;
	t_token	*second;

	first = create_tracked_token(TOKEN_WORD, "echo");
	second = create_tracked_token(TOKEN_PIPE, "|");
	TEST_ASSERT_EQUAL_PTR(first, g_test_token_list.head);
	TEST_ASSERT_EQUAL_PTR(second, first->next);
	TEST_ASSERT_NULL(second->next);
	TEST_ASSERT_EQUAL_PTR(second, g_test_token_list.tail);
}

/* 리스트가 NULL 인자를 거부하고 기존 상태를 유지하는지 확인한다. */
void	test_list_rejects_null_arguments_without_changing_the_list(void)
{
	char		*value;
	t_status	status;

	value = ft_strdup("echo");
	TEST_ASSERT_NOT_NULL(value);
	status = token_init(&g_test_token, TOKEN_WORD, value);
	if (status == OK)
		g_test_token_initialized = 1;
	else
		free(value);
	TEST_ASSERT_EQUAL_INT(OK, status);
	TEST_ASSERT_EQUAL_INT(FAIL, token_list_add_token(NULL, &g_test_token));
	TEST_ASSERT_EQUAL_INT(FAIL, token_list_add_token(&g_test_token_list, NULL));
	TEST_ASSERT_NULL(g_test_token_list.head);
}

/* Unity 테스트를 등록하고 실행한 뒤 테스트 결과를 반환한다. */
int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_token_init_stores_its_fields_and_starts_unlinked);
	RUN_TEST(test_new_token_list_is_empty);
	RUN_TEST(test_list_appends_tokens_in_insertion_order);
	RUN_TEST(test_list_rejects_null_arguments_without_changing_the_list);
	return (UNITY_END());
}
