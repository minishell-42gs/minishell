#include "env.h"
#include "libft.h"
#include "unity.h"

/* 이 파일의 테스트는 공유 fixture를 사용하지 않는다. */
void	setUp(void)
{
}

/* 각 테스트에서 직접 해제하므로 공통 정리 작업이 없다. */
void	tearDown(void)
{
}

/* env_init이 key와 value를 깊은 복사하는지 확인한다. */
void	test_env_init_copies_key_and_value(void)
{
	t_env	env;
	char	key[] = "PATH";
	char	value[] = "/bin";

	ft_memset(&env, 0, sizeof(env));
	TEST_ASSERT_EQUAL_INT(OK, env_init(&env, key, value));
	TEST_ASSERT_NOT_EQUAL_PTR(key, env.key);
	TEST_ASSERT_NOT_EQUAL_PTR(value, env.value);
	TEST_ASSERT_EQUAL_STRING("PATH", env.key);
	TEST_ASSERT_EQUAL_STRING("/bin", env.value);
	key[0] = 'X';
	value[0] = 'x';
	TEST_ASSERT_EQUAL_STRING("PATH", env.key);
	TEST_ASSERT_EQUAL_STRING("/bin", env.value);
	TEST_ASSERT_NULL(env.next);
	TEST_ASSERT_NOT_NULL(env.destroy);
	env.destroy(&env);
}

/* env 노드는 빈 문자열 value도 그대로 보관하고 destroy할 수 있는지 확인한다. */
void	test_env_init_accepts_empty_value(void)
{
	t_env	env;
	char	key[] = "EMPTY";
	char	value[] = "";

	ft_memset(&env, 0, sizeof(env));
	TEST_ASSERT_EQUAL_INT(OK, env_init(&env, key, value));
	TEST_ASSERT_EQUAL_STRING("EMPTY", env.key);
	TEST_ASSERT_EQUAL_STRING("", env.value);
	env.destroy(&env);
}

/* Unity 테스트를 등록하고 실행한 뒤 테스트 결과를 반환한다. */
int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_env_init_copies_key_and_value);
	RUN_TEST(test_env_init_accepts_empty_value);
	return (UNITY_END());
}
