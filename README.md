*This project has been created as part of the 42 curriculum by ringo, taegokim.*

# Minishell

## Description

Minishell is a small interactive shell written in C for the 42 curriculum. It reads commands, parses words and operators, expands variables, runs builtins or external programs, and connects commands with pipelines. The mandatory features include quoting, environment variables, redirections, heredocs, and the seven required builtins.

Commands retain their original words for expansion, redirections run in source order, standalone state-changing builtins run in the parent shell, and pipeline commands run in child processes.

## Instructions

Build from the repository root:

```sh
make -C Assignments
```

Run the shell:

```sh
./Assignments/minishell
```

The program links against GNU Readline. Run the test suites with:

```sh
make -C Tests test
make -C Tests integration
make -C Tests integration-signals
make -C Tests sanitize
make -C Tests memory
```

Bonus operators (`&&`, `||`, grouping parentheses, and wildcard expansion) are outside the mandatory implementation.

## Resources

- [42 Minishell subject](Materials/Subjects/minishell_en.md)
- [42 EvalHub Minishell checklist](https://www.42evalhub.com/common/minishell)
- [Bash manual](https://www.gnu.org/software/bash/manual/bash.html)
- [GNU Readline documentation](https://tiswww.case.edu/php/chet/readline/readline.html)

AI assistance was used to review the executor and error-handling design, implement and debug mandatory parsing, expansion, builtin, redirection, heredoc, and signal paths, and prepare regression tests and design notes. Behavior was checked against the project subject, Bash comparisons in the integration suite, Norminette, and the sanitizer and memory-check workflows.
