*This project has been created as part of the 42 curriculum by hyuckwon, taegokim.*

# Minishell

## Description

Minishell is a small interactive shell written in C, modeled on Bash. It reads a command line, splits it into words and operators, expands variables, and runs builtins or external programs, connecting commands through pipelines.

The mandatory part is implemented:

- Prompt with a working history (GNU Readline)
- Command lookup through `PATH`, relative paths and absolute paths
- Single and double quotes, `$VAR` and `$?` expansion
- Redirections `<`, `>`, `>>` and heredoc `<<`
- Pipes `|`
- Builtins `echo -n`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- `ctrl-C`, `ctrl-D` and `ctrl-\` behaving as in Bash

The bonus part (`&&`, `||`, parentheses, wildcards) is not implemented.

## Instructions

Requirements: `cc`, `make` and the GNU Readline library (for example `libreadline-dev` on Debian/Ubuntu).

Build (the bundled `libft` is compiled first):

```sh
make
```

Run:

```sh
./minishell
```

Other rules: `make clean`, `make fclean`, `make re`.

## Resources

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)
- [GNU Readline Library](https://tiswww.case.edu/php/chet/readline/readline.html)
- Linux man pages: `fork(2)`, `execve(2)`, `pipe(2)`, `dup2(2)`, `waitpid(2)`, `sigaction(2)`

AI was used as a review and debugging aid: to review the executor and error-handling design, to help track down bugs (memory and file descriptor leaks in child processes, heredoc and signal edge cases), and to suggest test cases. All behavior was checked against Bash and Norminette, and memory was checked with valgrind.
