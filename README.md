# Minishell

A minimalist UNIX shell implementation that serves as an introduction to the foundational concepts of command interpreters. While shells may appear simple on the surface, they exhibit precise and well-defined behaviors in nearly every interaction, most of which require careful implementation.

![Minishell Demo](https://github.com/user-attachments/assets/58d614f4-495b-4819-ae0a-f8d8df573287)

## About The Project

This project involves creating a simple yet functional UNIX-style shell.

The development of shells is deeply intertwined with the history of computing itself. In the early days of computing, developers recognized that interacting with computers through physical switches and binary code was impractical and inefficient. This realization led to the creation of software that could communicate with computers using interactive command lines in languages more aligned with human communication.

Through building Minishell, you'll gain insight into the computing challenges that existed before modern graphical interfaces, experiencing firsthand the fundamental problems that pioneers in computing had to solve. This project offers a practical understanding of system programming, process management, and the architecture behind the command-line interfaces we use daily.

## Features

The behavior of this minishell is heavily based on the bash shell. For every feature, bash was used as the reference implementation.

### Builtin Commands

- `echo` &mdash; Supports the `-n` option.
- `cd` &mdash; Handles absolute and relative paths only.
- `pwd` &mdash; Displays the current working directory (no options).
- `export` &mdash; Exports variables (no options).
- `unset` &mdash; Removes variables (no options).
- `env` &mdash; Displays the environment variables (no options or arguments).
- `exit` &mdash; Terminates the shell (no options).

### Shell Functionality

- Display a prompt while waiting for a new command.
- Working command history (navigable with up/down arrows).
- Find and launch executables via `PATH` or absolute/relative paths.
- Environment variable expansion (`$VAR`).
- Handle `$?` for the last command's exit status.
- Single Quotes (`''`): Prevents interpretation of all metacharacters.
- Double Quotes (`""`): Prevents interpretation of metacharacters except `$`.
- `<` Redirect input.
- `>` Redirect output (overwrite).
- `<<` Here Documents (reads input until a delimiter).
- `>>` Redirect output (append).
- `|` Pipes to connect the output of one command to the input of the next.
- `ctrl-C` (SIGINT): Prints a new prompt on a new line.
- `ctrl-D` (EOF): Exits the shell.
- `ctrl-\` (SIGQUIT): Does nothing.

> 💡 _Extra:_ Optionally supports logical operators `&&`, `||`, subshells using parentheses `()`, and wildcards `*` for filename expansion.

## Getting Started

### Prerequisites

- GCC or Clang (C compiler)
- GNU Make
- `readline` library

### Build

```sh
cd minishell
make
```

### Usage

```sh
./minishell
```

## What I Learned

- System programming basics: process management, forking, and executing commands
- Terminal I/O handling with signals and job control
- Memory management and error handling in C
- Implementing shell builtins and parsing user input

## Personal Experience

Developing Minishell was a deep dive into the inner workings of UNIX shells and the C programming language. It improved my abilities to debug, design robust software, and understand operating system concepts at a low level.

## License

This project is licensed under the MIT License.

## Contributors

- Sgmih ([sgmih@student.42.fr](mailto:sgmih@student.42.fr))
- Melkess ([melkess@student.42.fr](mailto:melkess@student.42.fr))

---
> Project developed as part of the 42 curriculum.
```
