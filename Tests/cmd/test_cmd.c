#include "cmd.h"
#include "libft.h"
#include "unity.h"
#include <stdlib.h>

static t_cmd	g_test_cmd;
static int	g_test_cmd_initialized;

/* 각 테스트 전에 비어 있는 명령 객체를 초기화한다. */
void	setUp(void)
{
	g_test_cmd_initialized = 0;
	TEST_ASSERT_EQUAL_INT(OK, cmd_init(&g_test_cmd));
	g_test_cmd_initialized = 1;
}

/* 각 테스트가 끝난 뒤 명령이 소유한 인자 배열을 정리한다. */
void	tearDown(void)
{
	if (g_test_cmd_initialized)
		g_test_cmd.destroy(&g_test_cmd);
}

/* 문자열을 복제한 뒤 테스트 명령의 인자로 추가한다. */
static void	append_argument(const char *argument)
{
	char		*copy;
	t_status	status;

	copy = ft_strdup(argument);
	TEST_ASSERT_NOT_NULL(copy);
	status = cmd_append_argv(&g_test_cmd, copy);
	if (status != OK)
		free(copy);
	TEST_ASSERT_EQUAL_INT(OK, status);
}

/* 새 명령이 빈 argv와 연결되지 않은 상태로 초기화되는지 확인한다. */
void	test_cmd_init_creates_an_empty_unlinked_command(void)
{
	TEST_ASSERT_NOT_NULL(g_test_cmd.argv);
	TEST_ASSERT_NULL(g_test_cmd.argv[0]);
	TEST_ASSERT_NULL(g_test_cmd.redirs);
	TEST_ASSERT_NULL(g_test_cmd.next);
	TEST_ASSERT_NOT_NULL(g_test_cmd.destroy);
}

/* 명령 인자가 삽입 순서대로 저장되고 NULL로 끝나는지 확인한다. */
void	test_cmd_appends_ls_and_option_in_order(void)
{
	append_argument("ls");
	append_argument("-l");
	TEST_ASSERT_EQUAL_STRING("ls", g_test_cmd.argv[0]);
	TEST_ASSERT_EQUAL_STRING("-l", g_test_cmd.argv[1]);
	TEST_ASSERT_NULL(g_test_cmd.argv[2]);
}

/* NULL 인자를 거부해 기존의 빈 argv를 유지하는지 확인한다. */
void	test_cmd_rejects_null_argument_without_changing_argv(void)
{
	TEST_ASSERT_EQUAL_INT(FAIL, cmd_append_argv(&g_test_cmd, NULL));
	TEST_ASSERT_NULL(g_test_cmd.argv[0]);
}

/* Unity 테스트를 등록하고 실행한 뒤 테스트 결과를 반환한다. */
int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_cmd_init_creates_an_empty_unlinked_command);
	RUN_TEST(test_cmd_appends_ls_and_option_in_order);
	RUN_TEST(test_cmd_rejects_null_argument_without_changing_argv);
	return (UNITY_END());
}
