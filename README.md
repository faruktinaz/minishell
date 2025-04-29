# MiniShell

## Project Overview

MiniShell is a simple Unix-like command-line shell implemented in C as a learning exercise. This project demonstrates how to:

- Launch external commands using the `exec` family of functions
- Implement pipelines (`|`) to connect processes
- Perform standard input/output redirections (`<`, `>`, `>>`)
- Handle environment variables
- Recognize and execute built-in commands (e.g., `cd`, `echo`, `export`, `unset`, `pwd`, `env`, `exit`)
- Manage child processes and handle signals
- Support heredoc (`<<`) syntax

Key components include a **lexer**, **parser**, **executor**, **signal handler**, and integration with GNU Readline for interactive input.

---

## Features

1. **Command Parsing**
   - Tokenization of input, respecting single and double quotes
   - Environment variable expansion (e.g., `$VAR`)
   - Heredoc support (`<<`)

2. **Command Execution**
   - Running external programs and scripts
   - Handling multiple pipelines
   - Redirecting input and output to files

3. **Built-in Commands**
   - `cd` — Change directory
   - `echo` — Display a line of text
   - `pwd` — Print working directory
   - `export` — Set environment variables
   - `unset` — Unset environment variables
   - `env` — List environment variables
   - `exit` — Exit the shell

4. **Signal Handling**
   - Custom handling for `SIGINT` (Ctrl+C) and `SIGQUIT` (Ctrl+\)

5. **Dependencies**
   - GNU Readline (v8.2) for command-line editing
   - Custom `libft` library (constructed as part of the 42 curriculum)

---

## Repository Structure

```bash
minishell/
├── srcs/
│   ├── lexer/            # Tokenizes input strings
│   ├── parser/           # Builds command trees from tokens
│   ├── exec/             # Executes commands and pipelines
│   ├── builtins/         # Implementation of built-in commands
│   ├── libft/            # Custom C utility library
│   └── redirections.c    # Functions for I/O redirection
├── readline/             # Automatically downloaded and built GNU Readline sources
├── Makefile              # Build targets and dependency rules
├── leaks.sh              # Script to check for memory leaks
└── README.md             # Project documentation
``` 

---

## Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/faruktinaz/minishell.git
   cd minishell
   ```

2. Build the project using the provided Makefile:

   ```bash
   make
   ```

   This command will download and compile the Readline library and build the `minishell` executable using the `libft` utility library.

---

## Usage

Start the shell by running:

```bash
./minishell
```

Within the shell, you can:

- Execute standard Unix commands (e.g., `ls`, `grep`, `cat`)
- Use pipelines and redirections
- Test built-in commands like `cd`, `export`, and `exit`

To monitor memory leaks while the shell is running, execute in another terminal:

```bash
./leaks.sh
```

---

## Testing & Debugging

- The `leaks.sh` script periodically checks running `minishell` processes for memory leaks.
- Compiler flags `-Wall -Wextra -Werror` are enabled to catch warnings and treat them as errors.

---


