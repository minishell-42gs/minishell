#include "env.h"
#include "libft.h"
#include "unity.h"

static t_env_list	g_env_list;
static t_env		g_path;
static t_env		g_path_extra;
static int			g_nodes_initialized;

/* get 구현만 검증할 수 있도록 노드를 직접 연결한 리스트를 만든다. */
void	setUp(void)
{
	char	*path_key;
	char	*path_value;
	char	*extra_key;
	char	*extra_value;

	ft_memset(&g_env_list, 0, sizeof(g_env_list));
	ft_memset(&g_path, 0, sizeof(g_path));
	ft_memset(&g_path_extra, 0, sizeof(g_path_extra));
	g_nodes_initialized = 0;
	path_key = ft_strdup("PATH");
	path_value = ft_strdup("/bin");
	extra_key = ft_strdup("PATH_EXTRA");
	extra_value = ft_strdup("/custom");
	TEST_ASSERT_NOT_NULL(path_key);
	TEST_ASSERT_NOT_NULL(path_value);
	TEST_ASSERT_NOT_NULL(extra_key);
	TEST_ASSERT_NOT_NULL(extra_value);
	TEST_ASSERT_EQUAL_INT(OK, env_init(&g_path, path_key, path_value));
	TEST_ASSERT_EQUAL_INT(OK, env_init(&g_path_extra, extra_key, extra_value));
	g_path.next = &g_path_extra;
	g_env_list.head = &g_path;
	g_nodes_initialized = 1;
}

/* stack에 있는 노드는 free하지 않고, 노드가 소유한 문자열만 해제한다. */
void	tearDown(void)
{
	if (g_nodes_initialized)
	{
		g_path.destroy(&g_path);
		g_path_extra.destroy(&g_path_extra);
	}
}

/* 첫 노드의 key를 직접 조회해 value를 반환하는지 확인한다. */
void	test_env_list_get_impl_returns_value_from_first_node(void)
{
	TEST_ASSERT_EQUAL_STRING("/bin", env_list_get_impl(&g_env_list, "PATH"));
}

/* 접두사가 같아도 정확히 일치하는 key의 value를 반환하는지 확인한다. */
void	test_env_list_get_impl_requires_exact_key_match(void)
{
	TEST_ASSERT_EQUAL_STRING("/custom", env_list_get_impl(&g_env_list,
			"PATH_EXTRA"));
}

/* 없는 key와 NULL key는 NULL을 반환하는지 확인한다. */
void	test_env_list_get_impl_returns_null_for_unknown_or_null_key(void)
{
	TEST_ASSERT_NULL(env_list_get_impl(&g_env_list, "MISSING"));
	TEST_ASSERT_NULL(env_list_get_impl(&g_env_list, NULL));
}

/* Unity 테스트를 등록하고 실행한 뒤 테스트 결과를 반환한다. */
int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_env_list_get_impl_returns_value_from_first_node);
	RUN_TEST(test_env_list_get_impl_requires_exact_key_match);
	RUN_TEST(test_env_list_get_impl_returns_null_for_unknown_or_null_key);
	return (UNITY_END());
}
