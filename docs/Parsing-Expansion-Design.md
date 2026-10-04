# Parsing and Expansion Design

## Data flow

The lexer recognizes words and operators while respecting quote context. The parser consumes the token stream into a command list. Each command stores raw words and expanded argv separately, plus a linked list of redirections in source order.

```text
input line
  -> lexer tokens (raw word spelling + operator kind)
  -> parser (pipeline commands + ordered redirections)
  -> expansion (argv and redirection targets)
  -> heredoc collection
  -> executor
```

Expansion never feeds tokens back into the lexer. A variable whose value contains a pipe remains argument data and cannot create another pipeline.

## Quote-aware fields

The expander tracks single-quoted, double-quoted, and unquoted states.

- Single-quoted text is copied literally.
- Double-quoted text allows variable expansion and keeps its result in one field.
- Unquoted variable output is split on shell whitespace.
- Adjacent quoted and unquoted fragments join into the same word.
- Empty quoted fragments create an empty argument. An unset expansion by itself creates no field.
- Supported expansions are environment names and the previous command status, written as `$?`.

This mandatory-subset model does not implement wildcard expansion or configurable IFS.

## Redirection targets and heredoc delimiters

Redirection targets use quote removal and variable expansion, then require exactly one resulting field. Zero or multiple fields produce an ambiguous-redirection error before command execution.

A heredoc delimiter is quote-removed without variable expansion. The parser records whether the delimiter contained quotes; that decides whether body lines expand variables. Body expansion does not perform field splitting.

Syntax validation happens before expansion. Missing redirection operands and invalid pipe placement remain syntax errors even when variables could produce similar text.
