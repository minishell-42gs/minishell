# Signal Design

The shell has three signal contexts: prompt input, command execution, and heredoc input.

| Context | Parent behavior | Child behavior | Result |
|---|---|---|---|
| Prompt | SIGINT handler records the signal; a Readline event hook clears the current line and returns to a fresh prompt. SIGQUIT is ignored. | — | Ctrl-C sets status 130; Ctrl-\\ does nothing |
| Pipeline/external execution | Parent ignores SIGINT and SIGQUIT while waiting. | Child restores default signal behavior before command setup and exec. | Final command status is returned; signal exit maps to 128 + signal |
| Heredoc | The recorded SIGINT is checked between input lines; SIGQUIT remains ignored. | Collection stays in the shell process. | Ctrl-C cancels collection, skips execution, and sets status 130 |

Only one file-scope signal variable is used, with type `volatile sig_atomic_t`. The handler only stores the signal number. Readline integration and cleanup happen in normal control flow, outside the handler.

`Tests/integration/test_signals.py` uses a pseudo-terminal to exercise prompt, child, heredoc, and Ctrl-D behavior. Run it with `make -C Tests integration-signals`.
