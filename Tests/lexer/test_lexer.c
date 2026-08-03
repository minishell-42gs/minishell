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

/* lexer가 "ls -l"을 순서가 보존된 두 단어 토큰으로 만드는지 확인한다.
 * 명령어와 옵션이 하나로 합쳐지거나 순서가 바뀌면 parser는 실제 입력과
 * 다른 argv를 만들고, 사용자가 입력한 옵션이 엉뚱한 명령에 전달된다.
 */
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

/* lexer가 여러 공백을 명령 인자에 포함하지 않는지 확인한다.
 * "  ls   -l  "에서 공백을 값으로 보존하면 첫 토큰이 "  ls"가 되거나
 * 빈 토큰이 생겨, 정상적인 셸 입력이 잘못된 명령 이름으로 바뀐다.
 */
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

/* lexer가 NULL 입력과 NULL 출력 리스트를 거부하고 기존 리스트를 유지하는지 확인한다.
 * lexer_run(NULL, ...)을 역참조하면 즉시 크래시하고, 실패 전에 토큰을
 * 일부 추가하면 이후 parser가 불완전한 입력을 정상 입력으로 오인한다.
 */
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
	RUN_TEST(test_lexer_rejects_null_arguments);
	return (UNITY_END());
}
