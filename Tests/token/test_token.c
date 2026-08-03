#include "libft.h"
#include "token.h"
#include "unity.h"
#include <stdlib.h>

static t_token_factory	g_factory;
static t_token_list		g_test_token_list;
static t_token			g_test_token;
static int				g_test_token_initialized;

/* 각 테스트 전에 토큰 팩토리와 토큰 리스트를 초기화한다. */
void	setUp(void)
{
	g_test_token_initialized = 0;
	TEST_ASSERT_EQUAL_INT(OK, token_factory_init(&g_factory));
	TEST_ASSERT_EQUAL_INT(OK, token_list_init(&g_test_token_list));
}

/* 각 테스트가 끝난 뒤 생성된 토큰과 테스트 자원을 정리한다. */
void	tearDown(void)
{
	if (g_test_token_initialized)
		g_test_token.destroy(&g_test_token);
	if (g_test_token_list.head != NULL)
		g_test_token_list.destroy(&g_test_token_list);
	g_factory.destroy(&g_factory);
}

/* 토큰을 생성하고 테스트용 리스트에 등록해 정리할 수 있게 한다. */
static t_token	*create_tracked_token(const char *value)
{
	t_token		*token;
	t_status	status;

	token = token_factory_create(&g_factory, value);
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

/* 토큰의 type/value가 저장되고 next가 NULL로 초기화되는지 확인한다.
 * 이미 다른 노드를 가리키던 토큰을 재사용할 때 next를 지우지 않으면,
 * 리스트가 의도하지 않은 노드로 이어지거나 자기 자신을 가리켜 무한 순회한다.
 */
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

/* 팩토리가 입력 문자열을 복사해 독립적인 단어 토큰을 만드는지 확인한다.
 * 입력 버퍼 "echo"를 나중에 "xcho"로 바꿔도 토큰 값은 echo여야 한다.
 * 주소를 공유하면 lexer 입력이 재사용되거나 해제될 때 토큰 값도 오염된다.
 */
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

/* 팩토리가 단일 파이프 "|"를 파이프 토큰으로 분류하는지 확인한다.
 * 이를 단순한 WORD로 분류하면 parser가 파이프라인 경계를 놓치고, 앞뒤
 * 명령을 하나의 argv로 실행하려고 시도한다.
 */
void	test_factory_classifies_a_single_pipe_as_pipe(void)
{
	t_token	*token;

	token = create_tracked_token("|");
	TEST_ASSERT_EQUAL_INT(TOKEN_PIPE, token->type);
	TEST_ASSERT_EQUAL_STRING("|", token->value);
}

/* 이중 파이프 "||"가 단일 파이프 토큰으로 잘못 분류되지 않는지 확인한다.
 * 현재 지원하지 않는 연산자를 TOKEN_PIPE로 쪼개면, 유효하지 않은 입력이
 * 두 명령을 연결하는 파이프라인으로 오인되어 parser 동작이 왜곡된다.
 */
void	test_factory_does_not_classify_double_pipe_as_single_pipe(void)
{
	t_token	*token;

	token = create_tracked_token("||");
	TEST_ASSERT_EQUAL_INT(TOKEN_WORD, token->type);
}

/* 새 토큰 리스트가 비어 있고 소멸 함수를 제공하는지 확인한다.
 * head가 쓰레기 주소로 남으면 첫 토큰을 추가할 때 기존 메모리를 따라가고,
 * 종료 시 소유하지 않은 토큰을 해제하는 문제가 생긴다.
 */
void	test_new_token_list_is_empty(void)
{
	TEST_ASSERT_NULL(g_test_token_list.head);
	TEST_ASSERT_NOT_NULL(g_test_token_list.destroy);
}

/* 토큰 리스트가 삽입 순서대로 토큰을 연결하는지 확인한다.
 * "echo |"의 순서가 바뀌면 parser가 파이프의 앞뒤를 반대로 해석해,
 * 사용자가 입력한 명령 구조와 다른 실행 계획을 만든다.
 */
void	test_list_appends_tokens_in_insertion_order(void)
{
	t_token	*first;
	t_token	*second;

	first = create_tracked_token("echo");
	second = create_tracked_token("|");
	TEST_ASSERT_EQUAL_PTR(first, g_test_token_list.head);
	TEST_ASSERT_EQUAL_PTR(second, first->next);
	TEST_ASSERT_NULL(second->next);
}

/* NULL 리스트와 NULL 토큰을 거부하고 기존의 빈 리스트를 유지하는지 확인한다.
 * 예를 들어 token_list_add_token(NULL, token)은 리스트 포인터 역참조로
 * 크래시할 수 있고, 실패한 token을 head에 남기면 다음 파싱에 섞인다.
 */
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
	RUN_TEST(test_factory_creates_a_word_with_its_own_value_copy);
	RUN_TEST(test_factory_classifies_a_single_pipe_as_pipe);
	RUN_TEST(test_factory_does_not_classify_double_pipe_as_single_pipe);
	RUN_TEST(test_new_token_list_is_empty);
	RUN_TEST(test_list_appends_tokens_in_insertion_order);
	RUN_TEST(test_list_rejects_null_arguments_without_changing_the_list);
	return (UNITY_END());
}
