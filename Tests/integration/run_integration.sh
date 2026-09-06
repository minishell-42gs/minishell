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
	printf '%b' "$1" | $RUN_ENV "$MINISHELL" >"$TMP/ms_raw" 2>"$TMP/ms_err"
	ms_status=$?
	grep -v "$PROMPT_PATTERN" "$TMP/ms_raw" >"$TMP/ms_out"
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
