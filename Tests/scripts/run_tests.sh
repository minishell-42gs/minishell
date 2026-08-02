#!/bin/sh

mode="test"
if [ "$1" = "--sanitize" ] || [ "$1" = "--valgrind" ]; then
	mode=${1#--}
	shift
fi

run_test_binary()
{
	test_binary=$1
	if [ "$mode" = "sanitize" ]; then
		ASAN_OPTIONS=detect_leaks=0 "./$test_binary"
	elif [ "$mode" = "valgrind" ]; then
		valgrind --leak-check=full --show-leak-kinds=all \
			--errors-for-leak-kinds=all --error-exitcode=1 \
			"./$test_binary"
	else
		"./$test_binary"
	fi
}

total_suites=0
passed_suites=0
failed_suites=0
total_tests=0
failed_tests=0
failed_names=""

for test_binary in "$@"; do
	total_suites=$((total_suites + 1))
	if output=$(run_test_binary "$test_binary" 2>&1); then
		passed_suites=$((passed_suites + 1))
	else
		failed_suites=$((failed_suites + 1))
		failed_names="$failed_names $test_binary"
	fi
	printf '%s\n' "$output"
	test_count=$(printf '%s\n' "$output" | awk \
		'/^[0-9]+ Tests [0-9]+ Failures/ { count = $1 } \
		END { print count + 0 }')
	failure_count=$(printf '%s\n' "$output" | awk \
		'/^[0-9]+ Tests [0-9]+ Failures/ { count = $3 } \
		END { print count + 0 }')
	total_tests=$((total_tests + test_count))
	failed_tests=$((failed_tests + failure_count))
done

passed_tests=$((total_tests - failed_tests))
printf '\n========== TOTAL TEST SUMMARY ==========\n'
printf 'Suites: %d total, %d passed, %d failed\n' \
	"$total_suites" "$passed_suites" "$failed_suites"
printf 'Tests : %d total, %d passed, %d failed\n' \
	"$total_tests" "$passed_tests" "$failed_tests"
if [ "$failed_suites" -eq 0 ]; then
	printf 'Result: ALL OK\n'
else
	printf 'Failed:%s\n' "$failed_names"
	printf 'Result: FAILED\n'
fi
printf '========================================\n'
test "$failed_suites" -eq 0
