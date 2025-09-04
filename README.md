## Minishell :

A minimalist UNIX shell implementation that serves as an introduction to the foundational concepts of command interpreters. While shells may appear simple on the surface, they exhibit precise and well-defined behaviors in nearly every interaction, most of which require careful implementation.

![Minishell Demo](https://github.com/user-attachments/assets/58d614f4-495b-4819-ae0a-f8d8df573287)

### About The Project :

This project involves creating a simple yet functional UNIX-style shell.

The development of shells is deeply intertwined with the history of computing itself. In the early days of computing, developers recognized that interacting with computers through physical switches and binary code was impractical and inefficient. This realization led to the creation of software that could communicate with computers using interactive command lines in languages more aligned with human communication.

Through building Minishell, you'll gain insight into the computing challenges that existed before modern graphical interfaces, experiencing firsthand the fundamental problems that pioneers in computing had to solve. This project offers a practical understanding of system programming, process management, and the architecture behind the command-line interfaces we use daily.

### What I Learned : 

- **Technical Skills :**
    -  Process creation and management using fork(), execve(), wait().
    -  File descriptor manipulation and redirection.
    -  Pipe implementation and inter-process communication.
    -  Signal handling and terminal management.
    -  Memory management and leak prevention in C.
    -  Lexical analysis and tokenization of input.
    -  Environment variable management.

- **Soft Skills :**
    - Collaborative problem-solving and pair programming.
    - Time management and project planning.
    - Debugging complex system-level code.
    - Reading and understanding technical documentation.
    - Code optimization and efficiency considerations.

### Personal Experience : 

- Building Minishell was an intense and rewarding journey into the heart of UNIX systems. The project's core challenge was constructing a functional shell from scratch, which demanded a deep understanding of process management and meticulous memory discipline.

- We began by implementing lexical analysis and recursive parsing to break down user input into a logical syntax tree, a foundational step that taught me the critical importance of a robust parsing architecture.
  
- The most difficult hurdle was seamlessly managing processes and file descriptors, especially when implementing pipes and redirections. It felt like delicate digital plumbing, requiring us to carefully open, duplicate, and close descriptors without a single leak.

- A significant point of pride was implementing globbing (wildcards) without using glob(), pushing us to handle pattern matching and directory traversal manually.

- We further expanded the shell's capabilities by developing logic to evaluate command groups with priority, ensuring complex commands were executed with the correct precedence.

- Ultimately, this project gave me a profound appreciation for the hidden complexity within every terminal and the elegant, timeless design philosophy of UNIX.


### Project Overview : 

The behavior of this minishell is heavily based on the bash shell. For every feature, bash was used as the reference implementation, following the rules described below.

- must implement the following builtins exactly like in bash :
  - **`echo`** → Supports the **`-n`** option.
  - **`cd`** → Handles **absolute** and **relative** paths only.
  - **`pwd`** → Displays the current working directory (**no options**).
  - **`export`** → Exports variables (**no options**).
  - **`unset`** → Removes variables (**no options**).
  - **`env`** → Displays the environment variables (**no options or arguments**).
  - **`exit`** → Terminates the shell (**no options**).

> 💡 _Extra:_ You can optionally implement logical operators `&&` and `||` for conditional command execution.
 
- must handle the following functions:
  - Display a prompt while waiting for a new command.
  - Working command history (navigable with up/down arrows).
  - Find and launch executables via `PATH` or absolute/relative paths.
  - Environment variable expansion (`$VAR`).
  - Handle `$?` for the last command's exit status.
  - **Single Quotes ('')**: Prevents interpretation of all metacharacters.
  - **Double Quotes ("")**: Prevents interpretation of metacharacters except `$`.
  - `<` Redirect input.
  - `>` Redirect output (overwrite).
  - `<<` Here Documents (reads input until a delimiter).
  - `>>` Redirect output (append).
  - `|` Pipes to connect the output of one command to the input of the next.
  - `ctrl-C` (SIGINT): Prints a new prompt on a new line.
  - `ctrl-D` (EOF): Exits the shell.
  - `ctrl-\` (SIGQUIT): Does nothing.

> 💡 _Extra:_ You can support subshells using parentheses `()` and wildcards `*` for filename expansion.

## Contributors
- SOUKAINA GMIH
- MOHAMMED AMINE ELKESS
