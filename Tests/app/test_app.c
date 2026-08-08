#include "app.h"
#include "libft.h"
#include "unity.h"

/* 각 테스트 전에 필요한 공통 상태가 없으므로 아무 작업도 하지 않는다. */
void	setUp(void)
{
}

/* 각 테스트 뒤에 정리할 공통 자원이 없으므로 아무 작업도 하지 않는다. */
void	tearDown(void)
{
}

/* 앱 초기화가 환경 변수를 저장하고 종료 상태를 0으로 만드는지 확인한다. */
void	test_app_init_stores_envp_and_resets_last_status(void)
{
	char	*envp[] = {"PATH=/bin", NULL};
	t_app	app;

	app.envp = NULL;
	app.last_status = 42;
	TEST_ASSERT_EQUAL_INT(OK, app_init(&app, envp));
	TEST_ASSERT_EQUAL_PTR(envp, app.envp);
	TEST_ASSERT_EQUAL_INT(0, app.last_status);
}

/* 앱 초기화가 run과 destroy 함수 포인터를 배선하는지 확인한다. */
void	test_app_init_wires_run_and_destroy(void)
{
	char	*envp[] = {"PATH=/bin", NULL};
	t_app	app;

	ft_memset(&app, 0, sizeof(t_app));
	TEST_ASSERT_EQUAL_INT(OK, app_init(&app, envp));
	TEST_ASSERT_NOT_NULL(app.run);
	TEST_ASSERT_NOT_NULL(app.destroy);
}

/* 앱 초기화가 소유한 parsing_facade를 초기화하는지 확인한다. */
void	test_app_init_initializes_parsing_facade(void)
{
	char	*envp[] = {"PATH=/bin", NULL};
	t_app	app;

	ft_memset(&app, 0, sizeof(t_app));
	TEST_ASSERT_EQUAL_INT(OK, app_init(&app, envp));
	TEST_ASSERT_NOT_NULL(app.parsing_facade.destroy);
}

/* 초기화된 앱을 destroy로 정리해도 크래시가 없는지 확인한다. */
void	test_app_destroy_after_init_is_safe(void)
{
	char	*envp[] = {"PATH=/bin", NULL};
	t_app	app;

	ft_memset(&app, 0, sizeof(t_app));
	TEST_ASSERT_EQUAL_INT(OK, app_init(&app, envp));
	app.destroy(&app);
}

/* Unity 테스트를 등록하고 실행한 뒤 테스트 결과를 반환한다. */
int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_app_init_stores_envp_and_resets_last_status);
	RUN_TEST(test_app_init_wires_run_and_destroy);
	RUN_TEST(test_app_init_initializes_parsing_facade);
	RUN_TEST(test_app_destroy_after_init_is_safe);
	return (UNITY_END());
}
