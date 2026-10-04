#include "io_mgr.h"
#include "libft.h"
#include "unity.h"

static t_io_mgr	g_io_mgr;

void	setUp(void)
{
	ft_memset(&g_io_mgr, 0, sizeof(g_io_mgr));
}

void	tearDown(void)
{
	if (g_io_mgr.destroy != NULL)
		g_io_mgr.destroy(&g_io_mgr);
}

void	test_io_mgr_initializes_a_single_command_without_pipes(void)
{
	TEST_ASSERT_EQUAL_INT(OK, io_mgr_init(&g_io_mgr, 1));
	TEST_ASSERT_EQUAL_INT(0, g_io_mgr.pipe_count);
	TEST_ASSERT_NULL(g_io_mgr.pipes);
	TEST_ASSERT_NOT_NULL(g_io_mgr.get_fds);
	TEST_ASSERT_NOT_NULL(g_io_mgr.close_all);
	TEST_ASSERT_NOT_NULL(g_io_mgr.destroy);
}

void	test_io_mgr_returns_standard_io_sentinels_for_a_single_command(void)
{
	int	in_fd;
	int	out_fd;

	TEST_ASSERT_EQUAL_INT(OK, io_mgr_init(&g_io_mgr, 1));
	in_fd = 0;
	out_fd = 0;
	TEST_ASSERT_EQUAL_INT(OK, g_io_mgr.get_fds(&g_io_mgr, 0, &in_fd,
			&out_fd));
	TEST_ASSERT_EQUAL_INT(-1, in_fd);
	TEST_ASSERT_EQUAL_INT(-1, out_fd);
}

void	test_io_mgr_creates_one_pipe_for_two_commands(void)
{
	TEST_ASSERT_EQUAL_INT(OK, io_mgr_init(&g_io_mgr, 2));
	TEST_ASSERT_EQUAL_INT(1, g_io_mgr.pipe_count);
	TEST_ASSERT_NOT_NULL(g_io_mgr.pipes);
	TEST_ASSERT_GREATER_OR_EQUAL(0, g_io_mgr.pipes[0].read_fd);
	TEST_ASSERT_GREATER_OR_EQUAL(0, g_io_mgr.pipes[0].write_fd);
}

int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_io_mgr_initializes_a_single_command_without_pipes);
	RUN_TEST(test_io_mgr_returns_standard_io_sentinels_for_a_single_command);
	RUN_TEST(test_io_mgr_creates_one_pipe_for_two_commands);
	return (UNITY_END());
}
