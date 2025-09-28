# 🐚 Minishell

A simple shell implementation in C, based on POSIX documentation and inspired by Bash. This project recreates the basics of a Unix shell, handling commands, redirections, pipes, and more. The program launches and works even without environment variables.

## Features

- **Prompt and History**: Displays a prompt and maintains command history.
- **Executable Search**: Finds and runs executables via PATH or absolute/relative paths.
- **Quotes Handling**: Supports single and double quotes for meta-character prevention.
- **Redirections**: Implements `<`, `>`, `>>`, `heredoc` for input/output redirection.
- **Pipes**: Supports command pipelines with `|`.
- **Environment Variables**: Expands `$VAR` and `$?` for exit status.
- **Signals**: Handles Ctrl-C, Ctrl-D, Ctrl-\ like Bash.
- **Built-ins**: Includes `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`.
- **Bonus**: Wildcards `*` for current directory.

## Compilation and Usage

Compile with `make`. Run with `./minishell`.

## Responsibilities

### [datagore](https://github.com/datagore)
- Parsing and tokenization (tokenize.c, params.c, split_words.c)
- Execution engine (execution.c, command_exec.c, shell_exec.c)
- Environment variable handling and expansion
- String and utility functions (string.c, utils.c)
- Memory management (memory.c)
- Pipes, redirections, and subprocess management (redirect.c, subprocess.c, pipe_extra_input.c)

### [oliynykmax](https://github.com/oliynykmax)
- Built-in commands (builtin_*.c files)
- Heredoc implementation (heredoc.c, heredoc_utils.c)
- Signals and user interface (signals.c, user_interface.c)
- Prompt handling
- Custom fprintf implementation

## Important Tools Written

- **Memory Arena**: Efficient memory allocation in ping-pong style for shell operations.
- **Vector**: Dynamic array implementation for tokens and environment variables.
