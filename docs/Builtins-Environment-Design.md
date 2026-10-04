# Builtins and Environment Design

## Environment representation

The application owns one environment list shared with parsing and the executor. Each entry tracks its key, value, exported state, and whether a value was assigned. This distinguishes:

- `export NAME`: a shell variable with no value, omitted from child `envp`;
- `export NAME=`: an exported variable with an empty value;
- `export NAME=value`: an exported variable with a value.

The `env` builtin and child environment include only exported entries that have values. With no arguments, `export` prints exported entries in sorted order. `unset` removes an entry. Expansion reads the same list, so changes are visible to the next command.

## Parent and child execution

A single builtin runs in the parent, so `cd`, `export`, and `unset` persist. The executor saves stdin and stdout before applying redirections and restores them when the builtin returns, including on command errors.

A builtin in a pipeline runs in its child process. Its environment and working-directory changes disappear with that child. External commands receive an `envp` generated from the current exported environment.

## Builtin behavior

- `echo` prints arguments and supports repeated leading `-n` options.
- `cd` changes directory and updates `PWD` and `OLDPWD`; without an argument it uses `HOME`, and `cd -` prints the destination.
- `pwd` prints the current working directory.
- `export` validates identifiers, sets values, and supports declarations without values.
- `unset` removes named entries.
- `env` prints exported entries with values.
- `exit` uses the previous status without an argument, validates numeric input and argument count, and returns an exit request so the application can clean up normally.

Pipeline status is the final command's status. Signal termination maps to 128 plus the signal number.
