#include "env.h"
#include "libft.h"
#include "unity.h"

static t_env_list	g_env_list;
static char			**g_env_array;
static int			g_list_was_destroyed;

/* 새 key/value 복사본의 소유권을 list->set에 넘긴다. */
static t_status	set_pair(const char *key, const char *value)
{
	char		*key_copy;
	char		*value_copy;
	t_status	status;

	key_copy = ft_strdup(key);
	value_copy = ft_strdup(value);
	if (key_copy == NULL || value_copy == NULL)
		return (free(key_copy), free(value_copy), FAIL);
	status = g_env_list.set(&g_env_list, key_copy, value_copy);
	if (status != OK)
		return (free(key_copy), free(value_copy), FAIL);
	return (OK);
}

/* to_envp가 만든 NULL 종단 문자열 배열을 해제한다. */
static void	free_env_array(char **env_array)
{
	size_t	i;

	if (env_array == NULL)
		return ;
	i = 0;
	while (env_array[i] != NULL)
		free(env_array[i++]);
	free(env_array);
}

/* 각 테스트 전에 환경 변수 리스트를 초기화할 준비를 한다. */
void	setUp(void)
{
	ft_memset(&g_env_list, 0, sizeof(g_env_list));
	g_env_array = NULL;
	g_list_was_destroyed = 0;
}

/* 초기화가 시도된 리스트가 소유한 노드를 정리한다. */
void	tearDown(void)
{
	free_env_array(g_env_array);
	if (g_env_list.destroy != NULL && !g_list_was_destroyed)
		g_env_list.destroy(&g_env_list);
}

/* envp의 key와 value를 각각 새 문자열로 복사하는지 확인한다. */
void	test_env_list_init_copies_key_and_value(void)
{
	char	entry[] = "PATH=/bin";
	char	*envp[] = {entry, NULL};

	TEST_ASSERT_EQUAL_INT(OK, env_list_init(&g_env_list, envp));
	TEST_ASSERT_NOT_NULL(g_env_list.head);
	TEST_ASSERT_EQUAL_STRING("PATH", g_env_list.head->key);
	TEST_ASSERT_EQUAL_STRING("/bin", g_env_list.head->value);

	entry[0] = 'X';
	entry[5] = 'x';
	TEST_ASSERT_EQUAL_STRING("PATH", g_env_list.head->key);
	TEST_ASSERT_EQUAL_STRING("/bin", g_env_list.head->value);
}

/* 값이 빈 문자열인 NAME= 항목을 정상적인 환경 변수로 저장하는지 확인한다. */
void	test_env_list_init_preserves_empty_value(void)
{
	char	*envp[] = {"EMPTY=", NULL};

	TEST_ASSERT_EQUAL_INT(OK, env_list_init(&g_env_list, envp));
	TEST_ASSERT_NOT_NULL(g_env_list.head);
	TEST_ASSERT_EQUAL_STRING("EMPTY", g_env_list.head->key);
	TEST_ASSERT_NOT_NULL(g_env_list.head->value);
	TEST_ASSERT_EQUAL_STRING("", g_env_list.head->value);
}

/* key 뒤의 첫 번째 =만 구분자로 사용하고 나머지는 value에 보존하는지 확인한다. */
void	test_env_list_init_preserves_equals_in_value(void)
{
	char	*envp[] = {"ASSIGN=a=b=c", NULL};

	TEST_ASSERT_EQUAL_INT(OK, env_list_init(&g_env_list, envp));
	TEST_ASSERT_NOT_NULL(g_env_list.head);
	TEST_ASSERT_EQUAL_STRING("ASSIGN", g_env_list.head->key);
	TEST_ASSERT_EQUAL_STRING("a=b=c", g_env_list.head->value);
}

/* =가 없는 malformed 항목은 리스트에 추가하지 않는지 확인한다. */
void	test_env_list_init_ignores_entry_without_equals(void)
{
	char	*envp[] = {"MALFORMED", "VALID=value", NULL};

	TEST_ASSERT_EQUAL_INT(OK, env_list_init(&g_env_list, envp));
	TEST_ASSERT_NOT_NULL(g_env_list.head);
	TEST_ASSERT_EQUAL_STRING("VALID", g_env_list.head->key);
	TEST_ASSERT_EQUAL_STRING("value", g_env_list.head->value);
	TEST_ASSERT_NULL(g_env_list.head->next);
}

/* 이름이 빈 =value 항목도 유효한 환경 변수로 추가하지 않는지 확인한다. */
void	test_env_list_init_ignores_entry_with_empty_key(void)
{
	char	*envp[] = {"=invalid", "VALID=value", NULL};

	TEST_ASSERT_EQUAL_INT(OK, env_list_init(&g_env_list, envp));
	TEST_ASSERT_NOT_NULL(g_env_list.head);
	TEST_ASSERT_EQUAL_STRING("VALID", g_env_list.head->key);
	TEST_ASSERT_EQUAL_STRING("value", g_env_list.head->value);
	TEST_ASSERT_NULL(g_env_list.head->next);
}

/* NULL envp를 거부하고 빈 리스트를 유지하는지 확인한다. */
void	test_env_list_init_rejects_null_envp(void)
{
	TEST_ASSERT_EQUAL_INT(FAIL, env_list_init(&g_env_list, NULL));
	TEST_ASSERT_NULL(g_env_list.head);
}

/* 빈 envp를 성공적으로 처리하고 빈 리스트를 만드는지 확인한다. */
void	test_env_list_init_accepts_empty_envp(void)
{
	char	*envp[] = {NULL};

	TEST_ASSERT_EQUAL_INT(OK, env_list_init(&g_env_list, envp));
	TEST_ASSERT_NULL(g_env_list.head);
}

/* 초기화 시 공개 함수 포인터를 모두 연결하는지 확인한다. */
void	test_env_list_init_wires_all_operations(void)
{
	char	*envp[] = {NULL};

	TEST_ASSERT_EQUAL_INT(OK, env_list_init(&g_env_list, envp));
	TEST_ASSERT_NOT_NULL(g_env_list.get);
	TEST_ASSERT_NOT_NULL(g_env_list.set);
	TEST_ASSERT_NOT_NULL(g_env_list.unset);
	TEST_ASSERT_NOT_NULL(g_env_list.to_envp);
	TEST_ASSERT_NOT_NULL(g_env_list.destroy);
}

/* 등록된 key의 value를 반환하는지 확인한다. */
void	test_env_list_get_returns_value_for_existing_key(void)
{
	char	*envp[] = {"HOME=/home/tg", NULL};

	TEST_ASSERT_EQUAL_INT(OK, env_list_init(&g_env_list, envp));
	TEST_ASSERT_EQUAL_STRING("/home/tg", g_env_list.get(&g_env_list,
			"HOME"));
}

/* key 전체를 비교해 접두사가 같은 다른 key와 구분하는지 확인한다. */
void	test_env_list_get_requires_exact_key_match(void)
{
	char	*envp[] = {"PATH=/bin", "PATH_EXTRA=/custom", NULL};

	TEST_ASSERT_EQUAL_INT(OK, env_list_init(&g_env_list, envp));
	TEST_ASSERT_EQUAL_STRING("/bin", g_env_list.get(&g_env_list, "PATH"));
	TEST_ASSERT_EQUAL_STRING("/custom", g_env_list.get(&g_env_list,
			"PATH_EXTRA"));
}

/* 존재하지 않는 key와 NULL key는 NULL을 반환하는지 확인한다. */
void	test_env_list_get_returns_null_for_unknown_or_null_key(void)
{
	char	*envp[] = {"PATH=/bin", NULL};

	TEST_ASSERT_EQUAL_INT(OK, env_list_init(&g_env_list, envp));
	TEST_ASSERT_NULL(g_env_list.get(&g_env_list, "MISSING"));
	TEST_ASSERT_NULL(g_env_list.get(&g_env_list, NULL));
}

/* 새 key를 추가하고 빈 value도 보존하는지 확인한다. */
void	test_env_list_set_adds_new_key_with_empty_value(void)
{
	char	*envp[] = {NULL};

	TEST_ASSERT_EQUAL_INT(OK, env_list_init(&g_env_list, envp));
	TEST_ASSERT_EQUAL_INT(OK, set_pair("EMPTY", ""));
	TEST_ASSERT_NOT_NULL(g_env_list.head);
	TEST_ASSERT_EQUAL_STRING("EMPTY", g_env_list.head->key);
	TEST_ASSERT_EQUAL_STRING("", g_env_list.get(&g_env_list, "EMPTY"));
	TEST_ASSERT_NULL(g_env_list.head->next);
}

/* 같은 key를 set하면 노드를 중복하지 않고 value를 교체하는지 확인한다. */
void	test_env_list_set_replaces_existing_value_without_duplicate(void)
{
	char	*envp[] = {"PATH=/bin", NULL};

	TEST_ASSERT_EQUAL_INT(OK, env_list_init(&g_env_list, envp));
	TEST_ASSERT_EQUAL_INT(OK, set_pair("PATH", "/usr/bin"));
	TEST_ASSERT_EQUAL_STRING("/usr/bin", g_env_list.get(&g_env_list, "PATH"));
	TEST_ASSERT_NOT_NULL(g_env_list.head);
	TEST_ASSERT_NULL(g_env_list.head->next);
}

/* 잘못된 set 인자는 실패하고 기존 리스트를 보존하는지 확인한다. */
void	test_env_list_set_rejects_null_arguments(void)
{
	char	*envp[] = {"PATH=/bin", NULL};

	TEST_ASSERT_EQUAL_INT(OK, env_list_init(&g_env_list, envp));
	TEST_ASSERT_EQUAL_INT(FAIL, g_env_list.set(&g_env_list, NULL, "value"));
	TEST_ASSERT_EQUAL_INT(FAIL, g_env_list.set(&g_env_list, "KEY", NULL));
	TEST_ASSERT_EQUAL_STRING("/bin", g_env_list.get(&g_env_list, "PATH"));
	TEST_ASSERT_NULL(g_env_list.head->next);
}

/* unset이 중간 노드만 제거하고 이웃 노드를 보존하는지 확인한다. */
void	test_env_list_unset_removes_middle_key(void)
{
	char	*envp[] = {"FIRST=1", "MIDDLE=2", "LAST=3", NULL};

	TEST_ASSERT_EQUAL_INT(OK, env_list_init(&g_env_list, envp));
	TEST_ASSERT_EQUAL_INT(OK, g_env_list.unset(&g_env_list, "MIDDLE"));
	TEST_ASSERT_EQUAL_STRING("1", g_env_list.get(&g_env_list, "FIRST"));
	TEST_ASSERT_NULL(g_env_list.get(&g_env_list, "MIDDLE"));
	TEST_ASSERT_EQUAL_STRING("3", g_env_list.get(&g_env_list, "LAST"));
	TEST_ASSERT_EQUAL_STRING("FIRST", g_env_list.head->key);
	TEST_ASSERT_EQUAL_STRING("LAST", g_env_list.head->next->key);
	TEST_ASSERT_NULL(g_env_list.head->next->next);
}

/* 첫 노드를 삭제해 head를 다음 노드로 갱신하는지 확인한다. */
void	test_env_list_unset_updates_head(void)
{
	char	*envp[] = {"FIRST=1", "LAST=2", NULL};

	TEST_ASSERT_EQUAL_INT(OK, env_list_init(&g_env_list, envp));
	TEST_ASSERT_EQUAL_INT(OK, g_env_list.unset(&g_env_list, "FIRST"));
	TEST_ASSERT_NOT_NULL(g_env_list.head);
	TEST_ASSERT_EQUAL_STRING("LAST", g_env_list.head->key);
	TEST_ASSERT_NULL(g_env_list.head->next);
}

/* 없는 key를 unset해도 bash처럼 성공하고 리스트를 유지하는지 확인한다. */
void	test_env_list_unset_ignores_unknown_key(void)
{
	char	*envp[] = {"PATH=/bin", NULL};

	TEST_ASSERT_EQUAL_INT(OK, env_list_init(&g_env_list, envp));
	TEST_ASSERT_EQUAL_INT(OK, g_env_list.unset(&g_env_list, "MISSING"));
	TEST_ASSERT_EQUAL_STRING("/bin", g_env_list.get(&g_env_list, "PATH"));
	TEST_ASSERT_NULL(g_env_list.head->next);
}

/* NULL key는 unset 대상이 아니므로 실패하는지 확인한다. */
void	test_env_list_unset_rejects_null_key(void)
{
	char	*envp[] = {"PATH=/bin", NULL};

	TEST_ASSERT_EQUAL_INT(OK, env_list_init(&g_env_list, envp));
	TEST_ASSERT_EQUAL_INT(FAIL, g_env_list.unset(&g_env_list, NULL));
	TEST_ASSERT_EQUAL_STRING("/bin", g_env_list.get(&g_env_list, "PATH"));
}

/* 리스트를 execve에 전달할 수 있는 NULL 종단 KEY=VALUE 배열로 만드는지 확인한다. */
void	test_env_list_to_envp_serializes_entries_in_order(void)
{
	char	*envp[] = {"PATH=/bin", "EMPTY=", "VALUE=a=b", NULL};

	TEST_ASSERT_EQUAL_INT(OK, env_list_init(&g_env_list, envp));
	g_env_array = g_env_list.to_envp(&g_env_list);
	TEST_ASSERT_NOT_NULL(g_env_array);
	TEST_ASSERT_EQUAL_STRING("PATH=/bin", g_env_array[0]);
	TEST_ASSERT_EQUAL_STRING("EMPTY=", g_env_array[1]);
	TEST_ASSERT_EQUAL_STRING("VALUE=a=b", g_env_array[2]);
	TEST_ASSERT_NULL(g_env_array[3]);
}

/* 빈 리스트도 execve 형식의 빈 NULL 종단 배열로 변환하는지 확인한다. */
void	test_env_list_to_envp_returns_empty_null_terminated_array(void)
{
	char	*envp[] = {NULL};

	TEST_ASSERT_EQUAL_INT(OK, env_list_init(&g_env_list, envp));
	g_env_array = g_env_list.to_envp(&g_env_list);
	TEST_ASSERT_NOT_NULL(g_env_array);
	TEST_ASSERT_NULL(g_env_array[0]);
}

/* 리스트 destroy가 모든 노드를 해제하고 재호출에도 안전한지 확인한다. */
void	test_env_list_destroy_clears_head_and_is_safe_twice(void)
{
	char	*envp[] = {"PATH=/bin", "HOME=/home/tg", NULL};

	TEST_ASSERT_EQUAL_INT(OK, env_list_init(&g_env_list, envp));
	TEST_ASSERT_NOT_NULL(g_env_list.head);
	g_env_list.destroy(&g_env_list);
	g_list_was_destroyed = 1;
	TEST_ASSERT_NULL(g_env_list.head);
	g_env_list.destroy(&g_env_list);
}

/* Unity 테스트를 등록하고 실행한 뒤 테스트 결과를 반환한다. */
int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_env_list_init_copies_key_and_value);
	RUN_TEST(test_env_list_init_preserves_empty_value);
	RUN_TEST(test_env_list_init_preserves_equals_in_value);
	RUN_TEST(test_env_list_init_ignores_entry_without_equals);
	RUN_TEST(test_env_list_init_ignores_entry_with_empty_key);
	RUN_TEST(test_env_list_init_rejects_null_envp);
	RUN_TEST(test_env_list_init_accepts_empty_envp);
	RUN_TEST(test_env_list_init_wires_all_operations);
	RUN_TEST(test_env_list_get_returns_value_for_existing_key);
	RUN_TEST(test_env_list_get_requires_exact_key_match);
	RUN_TEST(test_env_list_get_returns_null_for_unknown_or_null_key);
	RUN_TEST(test_env_list_set_adds_new_key_with_empty_value);
	RUN_TEST(test_env_list_set_replaces_existing_value_without_duplicate);
	RUN_TEST(test_env_list_set_rejects_null_arguments);
	RUN_TEST(test_env_list_unset_removes_middle_key);
	RUN_TEST(test_env_list_unset_updates_head);
	RUN_TEST(test_env_list_unset_ignores_unknown_key);
	RUN_TEST(test_env_list_unset_rejects_null_key);
	RUN_TEST(test_env_list_to_envp_serializes_entries_in_order);
	RUN_TEST(test_env_list_to_envp_returns_empty_null_terminated_array);
	RUN_TEST(test_env_list_destroy_clears_head_and_is_safe_twice);
	return (UNITY_END());
}
