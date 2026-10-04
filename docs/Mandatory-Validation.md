# Mandatory Validation

## Automated checks

| Command | Coverage |
|---|---|
| `make -C Assignments` | Required executable build |
| `make -C Tests test` | Unity unit and process-lifecycle tests |
| `make -C Tests integration` | End-to-end stdout and exit-status comparison with Bash |
| `make -C Tests integration-signals` | Interactive PTY signal and EOF behavior |
| `make -C Tests sanitize` | AddressSanitizer and UndefinedBehaviorSanitizer |
| `make -C Tests memory` | Valgrind leak/error check |
| `norminette Assignments/src Assignments/include` | Norminette source and header check |

The integration runner executes the real binary, filters the non-interactive prompt echo, and compares stdout and final status with Bash. It checks shell diagnostics directly where wording and status matter. Commands are bounded by a timeout.

## Feature matrix

Integration cases cover direct and PATH command lookup, missing and non-executable commands, single and multi-stage pipelines, large streams and early consumers, pipe syntax, quoting and concatenation, field splitting, environment changes, all seven builtins, redirections, heredoc expansion/quoting/multiplicity/EOF, and status propagation.

The PTY test covers Ctrl-C and Ctrl-\\ at the prompt, Ctrl-C and Ctrl-\\ while a child runs, Ctrl-C during heredoc collection, and Ctrl-D.

## Manual evaluator pass

1. Run standalone `cd`, then `pwd`; verify the parent directory changed.
2. Run `export TEST=value`, then `env | grep TEST`; verify the child receives the variable.
3. Run `echo hello > out | cat`; confirm explicit redirection and pipeline setup interact correctly.
4. Use quoted and unquoted heredoc delimiters; confirm only the latter expands variables.
5. Interrupt prompt input, a foreground command, and heredoc input with Ctrl-C; confirm a usable prompt returns.

The mandatory implementation does not support bonus operators, wildcard expansion, job control, command substitution, or full Bash grammar.
