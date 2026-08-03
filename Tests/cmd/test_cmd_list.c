#include "cmd.h"
#include "libft.h"
#include "unity.h"
#include <stdlib.h>

static t_cmd_list	g_test_cmd_list;
static t_cmd			*g_untracked_cmd;

/* 각 테스트 전에 비어 있는 명령 리스트를 초기화한다. */
void	setUp(void)
{
	g_untracked_cmd = NULL;
	TEST_ASSERT_EQUAL_INT(OK, cmd_list_init(&g_test_cmd_list));
}

/* 각 테스트가 끝난 뒤 리스트와 미등록 명령을 정리한다. */
void	tearDown(void)
{
	if (g_test_cmd_list.head != NULL)
		g_test_cmd_list.destroy(&g_test_cmd_list);
	if (g_untracked_cmd != NULL)
	{
		g_untracked_cmd->destroy(g_untracked_cmd);
		free(g_untracked_cmd);
	}
}

/* 새 명령을 생성하고 테스트용 명령 리스트에 등록한다. */
static t_cmd	*append_tracked_cmd(void)
{
	t_cmd		*cmd;
	t_status	status;

	cmd = ft_calloc(1, sizeof(t_cmd));
	TEST_ASSERT_NOT_NULL(cmd);
	status = cmd_init(cmd);
	if (status != OK)
		free(cmd);
	TEST_ASSERT_EQUAL_INT(OK, status);
	status = cmd_list_add_cmd(&g_test_cmd_list, cmd);
	if (status != OK)
	{
		cmd->destroy(cmd);
		free(cmd);
	}
	TEST_ASSERT_EQUAL_INT(OK, status);
	return (cmd);
}

/* 새 명령 리스트가 비어 있고 소멸 함수를 제공하는지 확인한다.
 * head가 초기화되지 않으면 첫 명령을 추가할 때 존재하지 않는 노드를
 * 따라가거나, 종료 시 초기화되지 않은 주소를 해제할 수 있다.
 */
void	test_cmd_list_init_creates_an_empty_list(void)
{
	TEST_ASSERT_NULL(g_test_cmd_list.head);
	TEST_ASSERT_NOT_NULL(g_test_cmd_list.destroy);
}

/* 명령 리스트가 삽입 순서대로 연결되는지 확인한다.
 * 예를 들어 "ls | wc"를 반대로 연결하면 파이프라인 실행 순서가 바뀌어
 * 사용자가 작성한 명령과 전혀 다른 결과가 나온다.
 */
void	test_cmd_list_appends_commands_in_insertion_order(void)
{
	t_cmd	*first;
	t_cmd	*second;

	first = append_tracked_cmd();
	second = append_tracked_cmd();
	TEST_ASSERT_EQUAL_PTR(first, g_test_cmd_list.head);
	TEST_ASSERT_EQUAL_PTR(second, first->next);
	TEST_ASSERT_NULL(second->next);
}

/* NULL 리스트와 NULL 명령을 거부하고 기존 리스트를 유지하는지 확인한다.
 * 어느 하나라도 허용하면 this->head 또는 new_node를 역참조하는 순간
 * 크래시가 발생하거나, 실패한 명령이 리스트에 부분적으로 남을 수 있다.
 */
void	test_cmd_list_rejects_null_arguments(void)
{
	g_untracked_cmd = ft_calloc(1, sizeof(t_cmd));
	TEST_ASSERT_NOT_NULL(g_untracked_cmd);
	TEST_ASSERT_EQUAL_INT(OK, cmd_init(g_untracked_cmd));
	TEST_ASSERT_EQUAL_INT(FAIL, cmd_list_add_cmd(NULL, g_untracked_cmd));
	TEST_ASSERT_EQUAL_INT(FAIL, cmd_list_add_cmd(&g_test_cmd_list, NULL));
	TEST_ASSERT_NULL(g_test_cmd_list.head);
}

/* Unity 테스트를 등록하고 실행한 뒤 테스트 결과를 반환한다. */
int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_cmd_list_init_creates_an_empty_list);
	RUN_TEST(test_cmd_list_appends_commands_in_insertion_order);
	RUN_TEST(test_cmd_list_rejects_null_arguments);
	return (UNITY_END());
}
