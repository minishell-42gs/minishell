#include "app.h"
#include "unity.h"

/* 각 테스트 전에 필요한 공통 상태가 없으므로 아무 작업도 하지 않는다. */
void	setUp(void)
{
}

/* 각 테스트 뒤에 정리할 공통 자원이 없으므로 아무 작업도 하지 않는다. */
void	tearDown(void)
{
}

/* 앱 초기화가 envp를 보존하고 last_status를 0으로 되돌리는지 확인한다.
 * 이전 명령의 상태가 42인 채 남으면 첫 실행부터 $?가 42로 오염되고,
 * envp를 잃으면 이후 PATH 기반 명령 검색이 실제 환경과 달라진다.
 */
void	test_app_init_stores_envp_and_resets_last_status(void)
{
	char	*envp[] = {"PATH=/bin", NULL};
	char	*argv[] = {"minishell", NULL};
	t_app	app;

	app.envp = NULL;
	app.last_status = 42;
	TEST_ASSERT_EQUAL_INT(OK, app_init(&app, 1, argv, envp));
	TEST_ASSERT_EQUAL_PTR(envp, app.envp);
	TEST_ASSERT_EQUAL_INT(0, app.last_status);
}

/* Unity 테스트를 등록하고 실행한 뒤 테스트 결과를 반환한다. */
int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_app_init_stores_envp_and_resets_last_status);
	return (UNITY_END());
}
