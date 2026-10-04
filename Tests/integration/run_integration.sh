#!/bin/sh

# 제출용 바이너리(Assignments/minishell)를 실제로 실행해 사용자 관점의 동작을 검증한다.
# 단위 테스트(test/sanitize/memory)는 함수 계약을 보고, 이 스크립트는 readline 루프부터
# 프로세스 종료 코드까지 이어지는 전체 경로를 bash와 비교한다.
#
# 스토리가 story 브랜치에 머지될 때마다 그 스토리가 약속하는 동작을 케이스로 추가한다.
# 이전 케이스는 지우지 않는다 (회귀 검사).
#
# 동작 방식
#   - 입력 줄을 stdin으로 넣는다. stdin이 tty가 아니면 readline이 프롬프트와 입력 줄을
#     stdout에 그대로 찍으므로 "minishell$ "로 시작하는 줄을 걸러낸 뒤 비교한다.
#   - stdin EOF 가 곧 Ctrl+D 이므로 "마지막 명령 뒤 Ctrl+D → 종료 코드 전달"도 함께 검증된다.

ROOT=$(cd "$(dirname "$0")/../.." && pwd)
MINISHELL="$ROOT/Assignments/minishell"
PROMPT_PATTERN='^minishell\$'
RUN_ENV=""
TIMEOUT=$(command -v timeout) || exit 1

if [ ! -x "$MINISHELL" ]; then
	printf 'minishell binary not found: %s\n' "$MINISHELL" >&2
	printf 'run "make -C Assignments" first\n' >&2
	exit 1
fi

cd "$ROOT" || exit 1

TMP=$(mktemp -d) || exit 1
trap 'rm -rf "$TMP"' EXIT INT TERM

total=0
failed=0
failed_names=""

# 입력을 minishell 에 넣고 프롬프트 줄을 제거한 stdout, stderr, 종료 코드를 파일로 남긴다.
run_minishell()
{
	printf '%b' "$1" | "$TIMEOUT" 10 $RUN_ENV "$MINISHELL" >"$TMP/ms_raw" 2>"$TMP/ms_err"
	ms_status=$?
	grep -v "$PROMPT_PATTERN" "$TMP/ms_raw" | sed 's/minishell\$ $//' >"$TMP/ms_out"
	ms_out=$(cat "$TMP/ms_out")
}

# 같은 입력을 bash 에 넣어 기준 출력과 종료 코드를 얻는다.
# RUN_ENV 가 PATH 를 비우는 경우에도 bash 자체는 찾아야 하므로 절대 경로로 부른다.
run_bash()
{
	printf '%b' "$1" | $RUN_ENV /bin/bash >"$TMP/bash_out" 2>/dev/null
	bash_status=$?
	bash_out=$(cat "$TMP/bash_out")
}

pass()
{
	total=$((total + 1))
	printf 'PASS  %s\n' "$1"
}

fail()
{
	total=$((total + 1))
	failed=$((failed + 1))
	failed_names="$failed_names
  - $1"
	printf 'FAIL  %s\n' "$1"
	printf '      %s\n' "$2"
}

# stdout 과 종료 코드가 bash 와 같은지 확인한다.
# $1: 케이스 이름  $2: 입력
same_as_bash()
{
	run_bash "$2"
	run_minishell "$2"
	if [ "$ms_out" = "$bash_out" ] && [ "$ms_status" = "$bash_status" ]; then
		pass "$1"
	else
		fail "$1" "exit: bash=$bash_status minishell=$ms_status"
		diff "$TMP/bash_out" "$TMP/ms_out" | head -5 | sed 's/^/      /'
	fi
}

# 종료 코드가 기대값과 같고 stdout 이 비어 있는지 확인한다.
# $1: 케이스 이름  $2: 입력  $3: 기대 종료 코드
expect_status()
{
	run_minishell "$2"
	if [ "$ms_status" = "$3" ] && [ -z "$ms_out" ]; then
		pass "$1"
	else
		fail "$1" "expected exit $3 with empty stdout, got exit $ms_status, stdout='$ms_out'"
	fi
}

# 없는 명령: 종료 코드 127, stderr 에 command not found, stdout 은 비어 있어야 한다.
# $1: 케이스 이름  $2: 입력
expect_not_found()
{
	run_minishell "$2"
	if [ "$ms_status" = "127" ] && [ -z "$ms_out" ] \
		&& grep -q "command not found" "$TMP/ms_err"; then
		pass "$1"
	else
		fail "$1" "exit=$ms_status stderr='$(cat "$TMP/ms_err")'"
	fi
}

# 셸이 직접 출력해야 하는 오류의 종료 코드·stderr 문구·stdout 비어 있음을 확인한다.
# $1: 케이스 이름  $2: 입력  $3: 기대 종료 코드  $4: stderr에 있어야 할 고정 문구
expect_shell_error()
{
	run_minishell "$2"
	if [ "$ms_status" = "$3" ] && [ -z "$ms_out" ] \
		&& grep -F -q "$4" "$TMP/ms_err"; then
		pass "$1"
	else
		fail "$1" "exit=$ms_status stdout='$ms_out' stderr='$(cat "$TMP/ms_err")'"
	fi
}

# ---------------------------------------------------------------------------
# v1: 외부 명령 한 개 실행 (PR #21 env_list, #29 pipe parsing, #31 executor)
# ---------------------------------------------------------------------------

same_as_bash 'ls -a: PATH 탐색 + 인자 전달' 'ls -a\n'
same_as_bash '/bin/ls -a: 직접 경로 실행' '/bin/ls -a\n'
same_as_bash 'ls -a Assignments: 인자 2개 전달' 'ls -a Assignments\n'
same_as_bash 'ls -la: 옵션 결합' 'ls -la\n'
same_as_bash '/bin/true: 종료 코드 0' '/bin/true\n'

expect_status '/bin/false: 출력 없이 종료 코드 1' '/bin/false\n' 1
expect_status 'Ctrl+D: 마지막 명령의 종료 코드로 종료' '/bin/true\n/bin/false\n' 1
expect_status '빈 줄과 공백 줄은 $? 를 바꾸지 않음' '/bin/false\n\n   \n' 1
expect_status '입력 없이 EOF: 종료 코드 0' '' 0

expect_not_found '없는 명령: 127 + stderr 메시지' 'no_such_cmd_xyz\n'
expect_status '없는 명령 뒤에도 셸이 계속 동작' 'no_such_cmd_xyz\n/bin/true\n' 0

RUN_ENV="env PATH=/nonexistent"
expect_not_found 'PATH 에 없는 명령: 127' 'ls -a\n'
same_as_bash 'PATH 무관하게 직접 경로는 실행됨' '/bin/ls -a\n'
RUN_ENV="env -i"
expect_not_found 'PATH 변수 자체가 없으면 127' 'ls -a\n'
RUN_ENV=""

# ---------------------------------------------------------------------------
# v2: 셸이 직접 처리하는 오류 (error facade, parser)
# ---------------------------------------------------------------------------

expect_shell_error '마지막 pipe: syntax error + 2' 'echo |\n' 2 \
	'syntax error near unexpected token'
expect_status 'syntax error 뒤에도 셸이 계속 동작' 'echo |\n/bin/true\n' 0

# ---------------------------------------------------------------------------
# v3: command resolver error distinctions
# ---------------------------------------------------------------------------

touch "$TMP/non_executable_cmd"
chmod 0644 "$TMP/non_executable_cmd"
mkdir "$TMP/directory_cmd"
mkdir "$TMP/path_only"
touch "$TMP/path_only/non_executable_cmd"
chmod 0644 "$TMP/path_only/non_executable_cmd"

expect_shell_error '비실행 직접 경로: Permission denied + 126' \
	"$TMP/non_executable_cmd\n" 126 'Permission denied'
expect_shell_error '없는 직접 경로: ENOENT + 127' \
	"$TMP/missing_cmd\n" 127 'No such file or directory'
expect_shell_error '직접 디렉터리: Is a directory + 126' \
	"$TMP/directory_cmd\n" 126 'Is a directory'
RUN_ENV="env PATH=$TMP/path_only"
expect_shell_error 'PATH의 비실행 파일: Permission denied + 126' \
	'non_executable_cmd\n' 126 'Permission denied'
RUN_ENV=""

# ---------------------------------------------------------------------------

# v4: 파이프 실행. timeout으로 교착도 테스트 실패로 처리한다.
same_as_bash '2단 파이프' '/bin/echo alpha | wc -c\n'
same_as_bash '3단 파이프' '/bin/echo alpha | cat | wc -c\n'
same_as_bash '공백 없는 파이프' '/bin/echo alpha|cat|wc -c\n'
same_as_bash '파일 읽기와 필터' 'cat Assignments/Makefile | grep SRC_DIR | wc -l\n'
same_as_bash '대용량 스트림' 'seq 1 100000 | cat | wc -l\n'
same_as_bash '일찍 닫히는 소비자' 'yes | head -n 1\n'
same_as_bash '중간 소비자의 조기 종료' 'yes | head -n 1 | wc -c\n'
same_as_bash '출력이 없는 생산자의 EOF' '/bin/true | cat | wc -c\n'
same_as_bash '앞 명령 실패, 마지막 성공' '/bin/false | /bin/true\n'
same_as_bash '마지막 명령 실패' '/bin/true | /bin/false\n'
same_as_bash '없는 첫 명령' 'no_such_cmd_xyz | cat | wc -c\n'
same_as_bash '없는 중간 명령' '/bin/echo alpha | no_such_cmd_xyz | wc -c\n'
expect_not_found '없는 마지막 명령' '/bin/echo alpha | no_such_cmd_xyz\n'
same_as_bash 'stderr는 파이프로 보내지 않음' 'ls /no_such_minishell_file | wc -c\n'
same_as_bash '파이프 이후 셸 입력과 출력 유지' '/bin/echo alpha | wc -c\n/bin/echo omega\n'
same_as_bash '마지막 프로세스가 먼저 종료' '/bin/sleep 0.1 | /bin/false\n'
expect_status '선두 pipe 오류' '| cat\n' 2
expect_status '연속 pipe 오류' 'cat | | wc\n' 2


# ---------------------------------------------------------------------------
# v5: quotes, expansion, builtins, redirections and heredocs
# ---------------------------------------------------------------------------

same_as_bash 'single/double quote behavior and concatenation' \
	'echo '\''$HOME'\''\necho "$HOME"\necho "pre"'\''mid'\''"post"\n'
same_as_bash 'exported variable splitting and quoted preservation' \
	'export MS_WORD='\''two words'\''\necho $MS_WORD\necho "$MS_WORD"\n'
same_as_bash '$? expansion tracks the preceding command' \
	'/bin/false\necho $?\n'
same_as_bash 'export reaches external commands and unset removes it' \
	'export MS_TEST=visible\nprintenv MS_TEST\nunset MS_TEST\nprintenv MS_TEST\n'
same_as_bash 'cd updates PWD and OLDPWD' 'cd /tmp\npwd\ncd -\npwd\n'
same_as_bash 'cd in a pipeline does not change the parent' \
	'cd / | pwd\npwd\n'
same_as_bash 'echo -n does not append a newline' 'echo -n no-newline\n'
same_as_bash 'exit without an argument keeps the previous status' \
	'/bin/false\nexit\n'
same_as_bash 'exit uses the requested numeric status' 'exit 42\n'
same_as_bash 'exit status is reduced to one byte' 'exit 256\n'
same_as_bash 'exit accepts a negative numeric status' 'exit -1\n'
same_as_bash 'exit with extra arguments leaves the shell running' \
	'exit 1 2\necho still-running\n'

same_as_bash 'output, append and input redirections' \
	"echo first > $TMP/redirection.txt\\necho second >> $TMP/redirection.txt\\ncat < $TMP/redirection.txt\\n"
same_as_bash 'external command with output redirection' \
	"/bin/echo external > $TMP/external.txt\\ncat $TMP/external.txt\\n"
same_as_bash 'redirection mixed with a pipeline' \
	"cat < $ROOT/Assignments/Makefile | wc -l\\n"
same_as_bash 'unquoted heredoc expands variables' \
	'cat << MINISHELL_EOF\n$HOME\nMINISHELL_EOF\n'
same_as_bash 'quoted heredoc delimiter suppresses expansion' \
	'cat << '\''MINISHELL_EOF'\''\n$HOME\nMINISHELL_EOF\n'
same_as_bash 'heredoc can feed a pipeline' \
	'cat << MINISHELL_EOF | wc -c\nheredoc\nMINISHELL_EOF\n'
same_as_bash 'all heredocs are collected and the last input wins' \
	'cat << FIRST << SECOND\nfirst body\nFIRST\nsecond body\nSECOND\n'
same_as_bash 'heredoc EOF runs with the collected body' \
	'cat << MINISHELL_EOF\npartial body\n'
same_as_bash 'multiple output redirections preserve order and side effects' \
	"echo ordered > $TMP/order-a.txt > $TMP/order-b.txt\\ncat $TMP/order-a.txt\\ncat $TMP/order-b.txt\\n"
same_as_bash 'builtin redirection restores parent stdout' \
	"pwd > $TMP/pwd.txt\\npwd\\ncat $TMP/pwd.txt\\n"
same_as_bash 'command-only redirection creates its file' \
	"> $TMP/empty-command.txt\\necho shell-still-running\\n"
same_as_bash 'command output redirection overrides its pipeline output' \
	"echo redirected > $TMP/pipeline-output.txt | cat\\ncat $TMP/pipeline-output.txt\\n"
expect_shell_error 'ambiguous redirection after field splitting' \
	'export MS_WORD='\''two words'\''\ncat < $MS_WORD\n' 1 'ambiguous redirect'
expect_shell_error 'missing input redirection target' 'cat <\n' 2 \
	'syntax error near unexpected token'

printf '\n========== INTEGRATION SUMMARY ==========\n'
printf 'Cases : %d total, %d passed, %d failed\n' \
	"$total" "$((total - failed))" "$failed"
if [ "$failed" -eq 0 ]; then
	printf 'Result: ALL OK\n'
else
	printf 'Failed:%s\n' "$failed_names"
	printf 'Result: FAILED\n'
fi
printf '=========================================\n'
test "$failed" -eq 0
