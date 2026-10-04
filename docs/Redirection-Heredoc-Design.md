# Redirection and Heredoc Design

## Ordered redirections

The parser stores redirections in source order. The executor connects pipeline stdin/stdout first, then applies each command's redirections from left to right. This preserves file side effects and the final stream choice: `echo text > first > second` creates/truncates both files and sends output to the second.

Input redirection opens read-only. Output redirection creates or truncates with mode 0644; append creates or appends with the same mode. Each opened descriptor is duplicated onto stdin or stdout and then closed.

A standalone builtin runs through a parent-side wrapper that saves stdin/stdout and restores them. A redirection-only command is valid and does not try to execute an empty command name.

## Heredoc lifecycle

All heredocs are collected in command and redirection order before the pipeline executes. A heredoc overridden by a later input redirection is still read. The final applicable input redirection determines the command's stdin.

For each heredoc:

1. Create a unique path with exclusive creation and mode 0600.
2. Open a separate reader descriptor, then unlink the path.
3. Read until the quote-removed delimiter, EOF, or Ctrl-C.
4. Expand body variables only when the delimiter was not quoted.
5. Close the writer and retain the reader descriptor on its redirection node.
6. During redirection setup, duplicate that descriptor onto stdin and close it.

A temporary file avoids filling a pipe before a consumer exists. Command destruction closes a heredoc descriptor that was not consumed. EOF before the delimiter emits a warning and executes with the collected body. Ctrl-C sets status 130, releases heredoc descriptors, and skips execution.

Interactive bodies use Readline's secondary prompt and are not added to history. Non-interactive bodies are read directly from stdin so Readline does not echo script input to stdout.
