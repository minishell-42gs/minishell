#include "libft.h"
#include "unity.h"
#include "util.h"

static char	**g_test_split;

/* 각 테스트 전에 정리할 문자열 배열 포인터를 초기화한다. */
void	setUp(void)
{
	g_test_split = NULL;
}

/* 테스트가 중간에 실패해도 할당된 문자열 배열을 정리한다. */
void	tearDown(void)
{
	if (g_test_split != NULL)
		free_split(g_test_split);
}

/* free_split이 NULL 입력을 안전하게 무시하는지 확인한다.
 * ft_split 실패나 선택적 정리 경로에서 free_split(NULL)이 호출될 수 있는데,
 * 이를 역참조하면 정상적인 오류 처리 자체가 이중 오류로 종료된다.
 */
void	test_free_split_accepts_null(void)
{
	free_split(NULL);
	TEST_PASS();
}

/* free_split이 "ls", "-l", NULL 배열과 각 문자열을 모두 정리하는지 확인한다.
 * 컨테이너만 해제하거나 NULL sentinel까지 순회하지 않으면 반복 호출마다
 * 문자열 누수가 쌓이고, 잘못된 주소를 free해 sanitizer 오류가 발생한다.
 */
void	test_free_split_releases_null_terminated_string_array(void)
{
	g_test_split = ft_calloc(3, sizeof(char *));
	TEST_ASSERT_NOT_NULL(g_test_split);
	g_test_split[0] = ft_strdup("ls");
	TEST_ASSERT_NOT_NULL(g_test_split[0]);
	g_test_split[1] = ft_strdup("-l");
	TEST_ASSERT_NOT_NULL(g_test_split[1]);
	free_split(g_test_split);
	g_test_split = NULL;
	TEST_PASS();
}

/* Unity 테스트를 등록하고 실행한 뒤 테스트 결과를 반환한다. */
int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_free_split_accepts_null);
	RUN_TEST(test_free_split_releases_null_terminated_string_array);
	return (UNITY_END());
}
