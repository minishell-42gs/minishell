#!/bin/sh

# Runs the submitted binary (Assignments/minishell) under valgrind and checks
# that both the parent and its children release memory and file descriptors.
# The evaluation scale requires every heap allocation to be freed before exit,
# so children that exit without reaching execve (command not found, failed
# redirections, builtins inside a pipeline, ...) must not leave still
# reachable blocks either.
#
# Pass criteria
#   - minishell processes (parent and forked children): no lost blocks,
#     no still reachable blocks, ERROR SUMMARY 0. Allocations made inside
#     readline are filtered out by readline.supp.
#   - every process, executed external commands included, holds no fd
#     other than 0, 1 and 2.

ROOT=$(cd "$(dirname "$0")/../.." && pwd)
MINISHELL="$ROOT/Assignments/minishell"
SUPP="$ROOT/Tests/integration/readline.supp"
VALGRIND=$(command -v valgrind) || { echo "valgrind not found" >&2; exit 1; }

if [ ! -x "$MINISHELL" ]; then
	printf 'minishell binary not found: %s\n' "$MINISHELL" >&2
	exit 1
fi

# Close fds inherited from make or other callers so they are not reported
# as leaks.
exec 3>&- 4>&- 5>&- 6>&- 7>&- 8>&- 9>&-

TMP=$(mktemp -d) || exit 1
trap 'rm -rf "$TMP"' EXIT INT TERM
mkdir "$TMP/dir"

total=0
failed=0

# Checks one per-process log, prints any problem and returns 1 if found.
check_log()
{
	log=$1
	bad=0
	if grep -q "Command: $MINISHELL" "$log"; then
		if grep -qE "(definitely|indirectly|possibly) lost: [1-9]|still reachable: [1-9]|ERROR SUMMARY: [1-9]" "$log"; then
			grep -E "lost: [1-9]|still reachable: [1-9]|ERROR SUMMARY: [1-9]" "$log" | sed 's/^/      /'
			bad=1
		fi
	fi
	# Ignore the log file fds that valgrind itself opens for --log-file.
	fds=$(grep -E "Open file descriptor ([3-9]|[1-9][0-9]+):" "$log" \
		| grep -v "$TMP/vg\.")
	if [ -n "$fds" ]; then
		grep -m1 "Command:" "$log" | sed 's/^/      /'
		printf '%s\n' "$fds" | sed 's/^/      /'
		bad=1
	fi
	return $bad
}

run_case()
{
	name=$1
	input=$2
	total=$((total + 1))
	rm -f "$TMP"/vg.*
	printf '%b' "$input" | (cd "$TMP" && "$VALGRIND" --leak-check=full \
		--show-leak-kinds=all --track-fds=yes --trace-children=yes \
		--suppressions="$SUPP" --log-file="$TMP/vg.%p" \
		"$MINISHELL" >/dev/null 2>&1)
	case_bad=0
	for log in "$TMP"/vg.*; do
		[ -f "$log" ] || continue
		check_log "$log" || case_bad=1
	done
	if [ "$case_bad" -eq 0 ]; then
		printf 'PASS  %s\n' "$name"
	else
		printf 'FAIL  %s\n' "$name"
		failed=$((failed + 1))
	fi
}

run_case "command not found in child" 'no_such_cmd\n'
run_case "failed input redirection in child" 'cat < nofile | cat\n'
run_case "failed output redirection in child" 'ls > /nonexistent/x | cat\n'
run_case "export builtin inside a pipeline" 'export A=1 | cat\n'
run_case "exit builtin inside a pipeline" 'exit 5 | cat\n'
run_case "redirection without a command in child" '> out | cat\n'
run_case "executing a directory in child" './dir\n'
run_case "command lookup without PATH" 'unset PATH\nls\n'
run_case "two heredocs and a pipe: no fd leaks to the next command" 'cat << A << B | wc -l\na\nA\nb\nB\n'
run_case "heredoc on both sides of a pipe" 'cat << A | cat << B\nx\nA\ny\nB\n'
run_case "successful pipeline" 'echo ok | cat | wc -c\n'
run_case "parent builtins then exit" 'export B=2\ncd /tmp\nexit 3\n'

printf '\n========== VALGRIND SUMMARY ==========\n'
printf 'Cases : %d total, %d passed, %d failed\n' "$total" $((total - failed)) "$failed"
if [ "$failed" -eq 0 ]; then
	printf 'Result: ALL OK\n'
else
	printf 'Result: FAILED\n'
fi
printf '======================================\n'
[ "$failed" -eq 0 ]
