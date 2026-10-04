# Minishell Mandatory Completion

## Goal and scope

This branch completes the mandatory portion of the 42 Minishell subject on top of the existing pipeline executor. The subject is the source of truth; Bash comparisons and the [42 EvalHub checklist](https://www.42evalhub.com/common/minishell) help exercise feature combinations.

- Work branch: `feature/complete-mandatory`
- Base: existing pipeline work on `feature/executor-with-pipe`
- Review target: `story`
- Subject: [Minishell](../Materials/Subjects/minishell_kr.md)
- Existing architecture notes: [Executor](Executor-Design.md), [Error handling](Error-Handling.md)
- Feature designs: [Parsing and expansion](Parsing-Expansion-Design.md), [builtins and environment](Builtins-Environment-Design.md), [redirections and heredocs](Redirection-Heredoc-Design.md), [signals](Signals-Design.md)

The implementation excludes bonus operators, wildcard expansion, job control, and command substitution.

## Completed implementation

| Area | Behavior |
|---|---|
| Input loop | Readline prompt and history, EOF exit, blank-line handling, last status |
| Lexer and parser | Words, pipes, input/output/append/heredoc operators; quote-aware syntax checks; ordered redirection nodes; redirection-only commands |
| Expansion | Single/double quotes, concatenation, empty quoted arguments, environment names and `$?`, unquoted field splitting, ambiguous redirect checks |
| Environment | Exported and unexported declarations, empty values, envp generation, export/unset updates |
| Builtins | echo, cd, pwd, export, unset, env, exit |
| Execution | PATH and direct-path commands; pipelines; standalone builtins in parent; pipeline builtins in children |
| Redirections | `<`, `>`, `>>` in input order after pipeline setup; parent stdio restoration |
| Heredocs | Multiple bodies collected before execution; quoted delimiter; body expansion; EOF warning; Ctrl-C cancellation; unlinked temporary files |
| Signals | Prompt Ctrl-C/Ctrl-\\ behavior, child signal status, heredoc interrupt; PTY regression coverage |
| Resources | Parent/child pipe closure, child collection, heredoc FD cleanup, error paths |

## Design decisions

1. Lexing and parsing preserve raw words. Expansion occurs after syntax is fixed, so text produced by a variable is never reparsed as an operator.
2. Each command keeps raw argv alongside expanded argv. The expander tracks quote context while building fields, preserving empty quoted fields and splitting unquoted expansion results.
3. Redirection nodes preserve source order. Pipeline descriptors are connected first; each command's redirections are then applied from left to right.
4. A standalone builtin or redirection-only command runs in the parent. The parent saves stdin/stdout, applies redirections, runs a builtin when present, then restores both descriptors.
5. Pipeline commands run in children. Their builtin state changes do not alter the parent shell.
6. Heredocs are collected in the parent before execution. Each body is written to an exclusively created temporary file, separately reopened for reading, and unlinked immediately.
7. The parent ignores execution signals while children restore default handling. A single `volatile sig_atomic_t` records a signal for prompt and heredoc control flow.

Detailed ownership and data flow are in the linked design notes.

## Validation

Bash-comparison integration cases exercise command lookup, quoting, expansion, builtins, pipelines, redirections, heredocs, syntax errors, and status propagation. A PTY test covers interactive signal behavior.

Run before review:

```sh
make -C Assignments
make -C Tests test
make -C Tests integration
make -C Tests integration-signals
make -C Tests sanitize
make -C Tests memory
norminette Assignments/src Assignments/include
```

See [Mandatory validation](Mandatory-Validation.md) for coverage. This records the mandatory implementation; it does not claim support for every behavior in a full shell.
