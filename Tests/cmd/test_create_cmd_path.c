#include "cmd.h"
#include "libft.h"
#include "unity.h"
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define TEST_DIR_TEMPLATE "/tmp/minishell_cmd_path_XXXXXX"
#define TEST_COMMAND "fixture_cmd"

static char	g_test_dir_a[sizeof(TEST_DIR_TEMPLATE)];
static char	g_test_dir_b[sizeof(TEST_DIR_TEMPLATE)];
static int	g_test_dir_a_created;
static int	g_test_dir_b_created;
static char	*g_test_file_a;
static char	*g_test_file_b;
static char	*g_test_nested_dir;
static char	*g_test_candidate_dir;
static char	*g_test_cmd_path;
static char	*g_test_path_env;
static char	g_test_original_cwd[PATH_MAX];
static int	g_test_cwd_changed;

/* 각 테스트에서 사용할 임시 경로와 현재 작업 디렉터리를 초기화한다. */
void	setUp(void)
{
	ft_strlcpy(g_test_dir_a, TEST_DIR_TEMPLATE, sizeof(g_test_dir_a));
	ft_strlcpy(g_test_dir_b, TEST_DIR_TEMPLATE, sizeof(g_test_dir_b));
	g_test_dir_a_created = 0;
	g_test_dir_b_created = 0;
	g_test_file_a = NULL;
	g_test_file_b = NULL;
	g_test_nested_dir = NULL;
	g_test_candidate_dir = NULL;
	g_test_cmd_path = NULL;
	g_test_path_env = NULL;
	g_test_cwd_changed = 0;
	if (getcwd(g_test_original_cwd, sizeof(g_test_original_cwd)) == NULL)
		TEST_FAIL_MESSAGE("failed to save the current working directory");
}

/* 테스트가 만든 파일, 디렉터리, 반환 경로를 모두 정리한다. */
void	tearDown(void)
{
	if (g_test_cwd_changed)
		chdir(g_test_original_cwd);
	if (g_test_file_a != NULL)
		unlink(g_test_file_a);
	if (g_test_file_b != NULL)
		unlink(g_test_file_b);
	if (g_test_nested_dir != NULL)
		rmdir(g_test_nested_dir);
	if (g_test_candidate_dir != NULL)
		rmdir(g_test_candidate_dir);
	free(g_test_file_a);
	free(g_test_file_b);
	free(g_test_nested_dir);
	free(g_test_candidate_dir);
	free(g_test_cmd_path);
	free(g_test_path_env);
	if (g_test_dir_a_created)
		rmdir(g_test_dir_a);
	if (g_test_dir_b_created)
		rmdir(g_test_dir_b);
}

/* 테스트 전용 디렉터리를 만든다. */
static void	make_test_directory(char *directory, int *created)
{
	if (mkdtemp(directory) == NULL)
	{
		TEST_FAIL_MESSAGE("failed to create a temporary directory");
		return ;
	}
	*created = 1;
}

/* 테스트 디렉터리 안에 PATH 후보로 사용할 하위 디렉터리를 만든다. */
static char	*create_fixture_directory(const char *parent, const char *name)
{
	char	*parent_slash;
	char	*directory;

	parent_slash = ft_strjoin(parent, "/");
	TEST_ASSERT_NOT_NULL(parent_slash);
	if (parent_slash == NULL)
		return (NULL);
	directory = ft_strjoin(parent_slash, name);
	free(parent_slash);
	TEST_ASSERT_NOT_NULL(directory);
	if (directory == NULL)
		return (NULL);
	if (mkdir(directory, 0755) < 0)
	{
		free(directory);
		TEST_FAIL_MESSAGE("failed to create a fixture directory");
		return (NULL);
	}
	return (directory);
}

/* 지정한 권한을 가진 빈 fixture 파일을 만든다. */
static char	*create_fixture_file(const char *directory, const char *name,
		mode_t mode)
{
	char	*directory_slash;
	char	*file_path;
	int		fd;

	directory_slash = ft_strjoin(directory, "/");
	TEST_ASSERT_NOT_NULL(directory_slash);
	if (directory_slash == NULL)
		return (NULL);
	file_path = ft_strjoin(directory_slash, name);
	free(directory_slash);
	TEST_ASSERT_NOT_NULL(file_path);
	if (file_path == NULL)
		return (NULL);
	fd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, mode);
	if (fd < 0)
	{
		free(file_path);
		TEST_FAIL_MESSAGE("failed to create a fixture file");
		return (NULL);
	}
	close(fd);
	if (chmod(file_path, mode) < 0)
	{
		free(file_path);
		TEST_FAIL_MESSAGE("failed to set fixture permissions");
		return (NULL);
	}
	return (file_path);
}

/* 두 디렉터리를 콜론으로 연결한 PATH 환경 변수를 만든다. */
static char	*create_path_env(const char *first, const char *second)
{
	char	*first_colon;
	char	*path_value;
	char	*path_env;

	first_colon = ft_strjoin(first, ":");
	TEST_ASSERT_NOT_NULL(first_colon);
	if (first_colon == NULL)
		return (NULL);
	path_value = ft_strjoin(first_colon, second);
	free(first_colon);
	TEST_ASSERT_NOT_NULL(path_value);
	if (path_value == NULL)
		return (NULL);
	path_env = ft_strjoin("PATH=", path_value);
	free(path_value);
	TEST_ASSERT_NOT_NULL(path_env);
	return (path_env);
}

/* PATH 항목이 현재 디렉터리를 가리킬 때 그 위치의 명령을 찾는지 확인한다. */
static void	assert_current_directory_is_searched(const char *path_value)
{
	char	*envp[2];

	make_test_directory(g_test_dir_a, &g_test_dir_a_created);
	g_test_file_a = create_fixture_file(g_test_dir_a, TEST_COMMAND, 0755);
	TEST_ASSERT_NOT_NULL(g_test_file_a);
	TEST_ASSERT_EQUAL_INT(0, chdir(g_test_dir_a));
	g_test_cwd_changed = 1;
	g_test_path_env = ft_strjoin("PATH=", path_value);
	TEST_ASSERT_NOT_NULL(g_test_path_env);
	envp[0] = g_test_path_env;
	envp[1] = NULL;
	g_test_cmd_path = create_cmd_path(TEST_COMMAND, envp);
	TEST_ASSERT_NOT_NULL(g_test_cmd_path);
	TEST_ASSERT_EQUAL_STRING("./" TEST_COMMAND, g_test_cmd_path);
}

/* 실행 가능한 절대 경로를 그대로 복제해 반환하는지 확인한다.
 * "/bin/sh"처럼 이미 slash를 포함한 명령은 PATH 검색으로 바꾸면 안 되며,
 * 입력 문자열을 그대로 반환하면 호출자가 반환값을 해제할 때 문제가 된다.
 */
void	test_create_cmd_path_returns_an_executable_direct_path(void)
{
	char	*cmd_name;

	cmd_name = "/bin/sh";
	g_test_cmd_path = create_cmd_path(cmd_name, NULL);
	TEST_ASSERT_NOT_NULL(g_test_cmd_path);
	TEST_ASSERT_EQUAL_STRING(cmd_name, g_test_cmd_path);
	TEST_ASSERT_NOT_EQUAL_PTR(cmd_name, g_test_cmd_path);
}

/* 실행 가능한 상대 경로도 직접 경로로 처리하는지 확인한다.
 * "./fixture_cmd"에는 slash가 있으므로 PATH=/missing이어도 찾아야 한다.
 * 이 경우를 PATH 검색으로만 처리하면 정상적인 상대 경로 실행이 실패한다.
 */
void	test_create_cmd_path_returns_an_executable_relative_path(void)
{
	char	*envp[2];

	make_test_directory(g_test_dir_a, &g_test_dir_a_created);
	make_test_directory(g_test_dir_b, &g_test_dir_b_created);
	g_test_nested_dir = create_fixture_directory(g_test_dir_a, "bin");
	g_test_file_a = create_fixture_file(g_test_nested_dir, TEST_COMMAND, 0755);
	g_test_file_b = create_fixture_file(g_test_dir_b, TEST_COMMAND, 0755);
	TEST_ASSERT_NOT_NULL(g_test_file_a);
	TEST_ASSERT_NOT_NULL(g_test_file_b);
	g_test_path_env = ft_strjoin("PATH=", g_test_dir_b);
	TEST_ASSERT_NOT_NULL(g_test_path_env);
	envp[0] = g_test_path_env;
	envp[1] = NULL;
	TEST_ASSERT_EQUAL_INT(0, chdir(g_test_dir_a));
	g_test_cwd_changed = 1;
	g_test_cmd_path = create_cmd_path("bin/" TEST_COMMAND, envp);
	TEST_ASSERT_NOT_NULL(g_test_cmd_path);
	TEST_ASSERT_EQUAL_STRING("bin/" TEST_COMMAND, g_test_cmd_path);
}

/* 존재하지만 실행 권한이 없는 직접 경로를 NULL로 거부하는지 확인한다.
 * 0644 파일을 실행 파일로 오인해 경로를 반환하면, 실제 execve 단계에서
 * 실행 권한 오류가 나고 경로 검색 함수의 결과만으로는 오류를 판별하기 어렵다.
 */
void	test_create_cmd_path_rejects_a_non_executable_direct_path(void)
{
	make_test_directory(g_test_dir_a, &g_test_dir_a_created);
	g_test_file_a = create_fixture_file(g_test_dir_a, TEST_COMMAND, 0644);
	TEST_ASSERT_NOT_NULL(g_test_file_a);
	TEST_ASSERT_NULL(create_cmd_path(g_test_file_a, NULL));
}

/* 직접 경로는 PATH에 같은 명령이 있어도 PATH 검색으로 대체하지 않는지 확인한다.
 * "/missing/fixture_cmd"가 실패했는데 PATH의 다른 fixture_cmd를 반환하면,
 * 사용자가 지정한 파일이 아닌 전혀 다른 프로그램을 실행하게 된다.
 */
void	test_create_cmd_path_does_not_fallback_to_path_for_a_direct_path(void)
{
	char	*envp[2];

	make_test_directory(g_test_dir_a, &g_test_dir_a_created);
	g_test_file_a = create_fixture_file(g_test_dir_a, TEST_COMMAND, 0755);
	TEST_ASSERT_NOT_NULL(g_test_file_a);
	g_test_path_env = ft_strjoin("PATH=", g_test_dir_a);
	TEST_ASSERT_NOT_NULL(g_test_path_env);
	envp[0] = g_test_path_env;
	envp[1] = NULL;
	TEST_ASSERT_NULL(create_cmd_path("/missing/fixture_cmd", envp));
}

/* PATH에 있는 첫 번째 실행 파일을 반환하는지 확인한다.
 * PATH=A:B이고 두 디렉터리에 같은 fixture_cmd가 있으면 A를 선택해야 한다.
 * 순서를 무시하면 PATH에 따라 달라져야 하는 명령 우선순위가 깨진다.
 */
void	test_create_cmd_path_uses_the_first_executable_in_path_order(void)
{
	char	*envp[2];

	make_test_directory(g_test_dir_a, &g_test_dir_a_created);
	make_test_directory(g_test_dir_b, &g_test_dir_b_created);
	g_test_file_a = create_fixture_file(g_test_dir_a, TEST_COMMAND, 0755);
	g_test_file_b = create_fixture_file(g_test_dir_b, TEST_COMMAND, 0755);
	TEST_ASSERT_NOT_NULL(g_test_file_a);
	TEST_ASSERT_NOT_NULL(g_test_file_b);
	g_test_path_env = create_path_env(g_test_dir_a, g_test_dir_b);
	envp[0] = g_test_path_env;
	envp[1] = NULL;
	g_test_cmd_path = create_cmd_path(TEST_COMMAND, envp);
	TEST_ASSERT_NOT_NULL(g_test_cmd_path);
	TEST_ASSERT_EQUAL_STRING(g_test_file_a, g_test_cmd_path);
}

/* 실행할 수 없는 PATH 후보를 건너뛰고 다음 후보를 찾는지 확인한다.
 * A/fixture_cmd가 0644이고 B/fixture_cmd가 0755일 때 B를 반환해야 한다.
 * 존재 여부만 확인하거나 첫 실패에서 중단하면 실행 가능한 후보를 놓친다.
 */
void	test_create_cmd_path_skips_a_non_executable_path_candidate(void)
{
	char	*envp[2];

	make_test_directory(g_test_dir_a, &g_test_dir_a_created);
	make_test_directory(g_test_dir_b, &g_test_dir_b_created);
	g_test_file_a = create_fixture_file(g_test_dir_a, TEST_COMMAND, 0644);
	g_test_file_b = create_fixture_file(g_test_dir_b, TEST_COMMAND, 0755);
	TEST_ASSERT_NOT_NULL(g_test_file_a);
	TEST_ASSERT_NOT_NULL(g_test_file_b);
	g_test_path_env = create_path_env(g_test_dir_a, g_test_dir_b);
	envp[0] = g_test_path_env;
	envp[1] = NULL;
	g_test_cmd_path = create_cmd_path(TEST_COMMAND, envp);
	TEST_ASSERT_NOT_NULL(g_test_cmd_path);
	TEST_ASSERT_EQUAL_STRING(g_test_file_b, g_test_cmd_path);
}

/* 상대 PATH 항목을 호출 시점의 현재 작업 디렉터리 기준으로 해석하는지 확인한다.
 * PATH=.에서 cd 이후의 디렉터리를 기준으로 삼지 않으면, 같은 PATH라도
 * 작업 디렉터리가 바뀐 뒤 다른 실행 파일을 찾거나 명령을 놓치게 된다.
 */
void	test_create_cmd_path_resolves_a_relative_path_from_the_current_directory(void)
{
	assert_current_directory_is_searched(".");
}

/* 명령어 이름이 비어 있으면 실행 파일 경로를 만들지 않는지 확인한다.
 * 빈 문자열을 "/bin/"처럼 PATH 디렉터리와 결합해 반환하면, 호출자는
 * 실행할 명령 대신 디렉터리를 execve하려고 시도하게 된다.
 */
void	test_create_cmd_path_rejects_an_empty_command_name(void)
{
	char	*envp[] = {"PATH=/bin", NULL};

	TEST_ASSERT_NULL(create_cmd_path("", envp));
}

/* PATH가 빈 문자열이면 순회할 디렉터리가 없어 NULL을 반환하는지 확인한다.
 * PATH=를 현재 디렉터리로 잘못 해석하면, 의도하지 않은 로컬 실행 파일을
 * 실행하게 되므로 PATH unset과 구분되는 빈 값의 의미가 깨진다.
 */
void	test_create_cmd_path_returns_null_for_an_empty_path(void)
{
	char	*envp[] = {"PATH=", NULL};

	TEST_ASSERT_NULL(create_cmd_path(TEST_COMMAND, envp));
}

/* 존재하지 않는 PATH 디렉터리를 조용히 건너뛰고 다음 디렉터리를 검색하는지 확인한다.
 * PATH=/no/such:<valid>에서 첫 디렉터리의 access 실패를 전체 검색 실패로
 * 처리하면, 뒤에 있는 정상 실행 파일까지 도달하지 못한다.
 */
void	test_create_cmd_path_skips_a_missing_path_directory(void)
{
	char	*envp[2];

	make_test_directory(g_test_dir_a, &g_test_dir_a_created);
	g_test_file_a = create_fixture_file(g_test_dir_a, TEST_COMMAND, 0755);
	TEST_ASSERT_NOT_NULL(g_test_file_a);
	g_test_path_env = ft_strjoin("PATH=/no/such:", g_test_dir_a);
	TEST_ASSERT_NOT_NULL(g_test_path_env);
	envp[0] = g_test_path_env;
	envp[1] = NULL;
	g_test_cmd_path = create_cmd_path(TEST_COMMAND, envp);
	TEST_ASSERT_NOT_NULL(g_test_cmd_path);
	TEST_ASSERT_EQUAL_STRING(g_test_file_a, g_test_cmd_path);
}

/* PATH의 어떤 디렉터리에도 명령이 없으면 NULL을 반환하는지 확인한다.
 * 검색 실패를 마지막 후보 경로나 디렉터리 경로로 오인하면, 없는 명령도
 * 실행 단계까지 넘어가 실제 오류 원인이 흐려진다.
 */
void	test_create_cmd_path_returns_null_when_no_path_entry_matches(void)
{
	char	*envp[2];

	make_test_directory(g_test_dir_a, &g_test_dir_a_created);
	g_test_path_env = ft_strjoin("PATH=", g_test_dir_a);
	TEST_ASSERT_NOT_NULL(g_test_path_env);
	envp[0] = g_test_path_env;
	envp[1] = NULL;
	TEST_ASSERT_NULL(create_cmd_path("missing_fixture_cmd", envp));
}

/* PATH 자체가 없으면 검색 결과를 만들지 않고 NULL을 반환하는지 확인한다.
 * HOME만 있는 envp에서 엉뚱한 환경 변수를 PATH로 취급하거나 초기화되지 않은
 * 주소를 읽으면 잘못된 경로가 반환되거나 검색 중 크래시가 발생할 수 있다.
 */
void	test_create_cmd_path_returns_null_without_a_path_environment(void)
{
	char	*envp[] = {"HOME=/tmp", NULL};

	TEST_ASSERT_NULL(create_cmd_path(TEST_COMMAND, envp));
}

/* Unity 테스트를 등록하고 실행한 뒤 테스트 결과를 반환한다. */
int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_create_cmd_path_returns_an_executable_direct_path);
	RUN_TEST(test_create_cmd_path_returns_an_executable_relative_path);
	RUN_TEST(test_create_cmd_path_rejects_a_non_executable_direct_path);
	RUN_TEST(test_create_cmd_path_does_not_fallback_to_path_for_a_direct_path);
	RUN_TEST(test_create_cmd_path_uses_the_first_executable_in_path_order);
	RUN_TEST(test_create_cmd_path_skips_a_non_executable_path_candidate);
	RUN_TEST(test_create_cmd_path_resolves_a_relative_path_from_the_current_directory);
	RUN_TEST(test_create_cmd_path_rejects_an_empty_command_name);
	RUN_TEST(test_create_cmd_path_returns_null_for_an_empty_path);
	RUN_TEST(test_create_cmd_path_skips_a_missing_path_directory);
	RUN_TEST(test_create_cmd_path_returns_null_when_no_path_entry_matches);
	RUN_TEST(test_create_cmd_path_returns_null_without_a_path_environment);
	return (UNITY_END());
}
