# minishell
## Description
Minishell is a minimalist UNIX shell written in C that replicates core bash features and 
provides an in-depth exploration of UNIX process management, pipes, signals, file descriptors, and parsing logic

## ✅ Key requirements
- Display a prompt when waiting for a new command.
- Have a working history.
- Search and launch the right executable.
- Use at most one global variable to indicate a received signal.

## 🚀 Features 
- Tokenization and parsing of input to handle quotes, pipes, and redirections.
- Execute commands with PATH, relative, and absolute paths
- Command history via 'readline'
- Handle quotes: ' (literal), " (allows variable expansion)
- Expand environment variables ($VAR, $?)
- Support redirections: <, >, >>, << (heredoc)
- Support pipes (|)
- Built-in commands: echo -n, cd, pwd, export, unset, env, exit
- Signal handling for Ctrl+C, Ctrl+D, and Ctrl+\ like Bash

## 🧱 Input Processing Flow
1. **Lexing** – Takes the full command line and splits it into logical units called tokens. For example, the input "echo "hello world" | wc -w" becomes a token list like [echo], [hello world], [|], [wc], [-w]. This lexer handles quotes and separates words based on spaces and operators.

2. **Parser** – After the lexer, the parser organizes the token list into a hierarchical structure called an Abstract Syntax Tree (AST). This tree represents the logical structure of the command line, respecting the precedence of operators (|, &&, ||, ;). This allows us to understand which commands to run and in what order. For instance, in a command with pipes, the tree will have a | operator node with a command on its left and another on its right.

3. **Prexecution** - In this phase, the AST is prepared for execution. Environment variables (like $PATH or $HOME) are expanded, I/O redirections and heredocs are managed, and absolute paths for commands are resolved.

3.1. _Expansion_ – Environment variables (`$VAR`, `$?`) are expanded, respecting shell quoting rules:
   - `'single quotes'` prevent expansion.
   - `"double quotes"` allow expansion.
4. **Execution** – traverses the Abstract Syntax Tree (AST) node by node. For each node, it takes the corresponding action:
- Executes external commands single and in pipes.
- If it is operator node, it manages the execution flow. For example, for a pipe (|), it creates left and right child processes and handles communication between them.
- Built-in commands are executed and is tracked their state and if they are a child process if they are part of a pipe. **Expected behavior:**

   - **echo** – Print the given arguments to standard output, separated by spaces, optionally suppressing the trailing newline with **-n**.
   - **env** – Display the list of environment variables.
   - **pwd** – Print the current working directory.
   - **export** – List or set environment variables in the shell.
   - **exit** – Terminate the shell with the given or last exit status.

This modular flow ensures predictable behavior and makes debugging easier.

## 💡 Usage

Supported operations include piping, redirections, environment management, and built-in command execution.

### ⚙️ Installation

**Clone the repo**:
```
   git clone https://github.com/yourusername/minishell.git
   cd minishell
```

**Built the project**:
```make```

**Run the shell**
```./minishell```

### Dependencies

## 🗂️ File Structure

```
minishell/
├── Makefile                 # Main build script (targets: NAME, all, clean, fclean, re)
├── README.md                # Public documentation: description, installation, usage 
├── docs/
│   └── TEST.md                  # Detailed test cases, results, and testing instructions
│   └── DEVELOPMENT.md           # Internal notes: features, progress tracking, decisions
│   └── GIT_WORKFLOW_GUIDE.md    # Internal use: create branches locally, sync main and move in branches.
├── include/                 # Header files
│   └── minishell.h          # Main header, includes other specific headers and common standard libraries
│   ├── structs.h
│   ├── lexer.h
│   ├── parser.h
│   ├── executor.h
│   ├── builtins.h
│   ├── signals.h
│   └── utils.h
│
├── libft/
│   ├── Makefile
│   ├── gnl/
│   ├── ft_printf/
│   └── ...
│
├── src/
│   ├── main.c
│
│   ├── lexer/               # Lexical analysis: tokenization and quote preservation
│   │   └── lexer.c
│
│   ├── parser/              # Parses tokens into commands, handles syntax & structure
│   │   └── parser.c
│
│   ├── exec/                # Execution logic, redirections, pipes, etc
│   │   └── exec.c, redir.c, pipe.c
│
│   ├── builtins/            # Built-in command implementations
│   │   └── cd.c, echo.c, env.c, export.c, exit.c, unset.c, pwd.c
│
│   ├── signals/             # Signal handling (Ctrl+C, Ctrl+D, etc.)
│   │   └── signals.c
│
│   └── utils/               # Helper functions
│       └── cleanup.c
│
└── tests/                   # Optional: Shell test scripts or CI/CD when push.
    ├── run_all_test.sh      #executes all listed scripts when 'make debug'
    ├── test_redirects.sh (example)
    └── test_builtins.sh (example)

```

👤 Autor
- Alicia Betancourt ([mainibet](https://github.com/mainibet))
- Tanmay Pandya

