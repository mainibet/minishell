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
1. **Lexing** – Raw input is split into tokens (commands, symbols, strings), preserving quotes and metacharacters.
2. **Parsing** – Tokens are grouped into structured command representations, handling pipes and redirections.
3. **Expansion** – Environment variables (`$VAR`, `$?`) are expanded, respecting shell quoting rules:
   - `'single quotes'` prevent expansion.
   - `"double quotes"` allow expansion.
4. **Execution** – Built-ins are handled internally; others use `execve`. Redirections and pipes are configured here.

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
├── DEVELOPMENT.md           # Internal notes: features, progress tracking, decisions
├── GIT_WORKFLOW_GUIDE.md    # Internal use: create branches locally, sync main and move in branches.
├── TEST.md                  # Detailed test cases, results, and testing instructions
│
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
├── libft/                   # Custom libft copy (mandatory if allowed)
│   ├── Makefile             # Builds libft separately before minishell
│   ├── gnl/                 # get_next_line implementation
│   ├── ft_printf/           # Custom ft_printf implementation
│   └── ...                  # Core libft functions (ft_strlen, ft_split, etc.)
│
├── src/                     # Source code organized by responsibility
│   ├── main.c               # Entry point (initialization, shell loop)
│
│   ├── lexer/               # Lexical analysis: tokenization and quote preservation
│   │   └── lexer.c, utils.c
│
│   ├── parser/              # Parses tokens into commands, handles syntax & structure
│   │   └── parser.c, ast.c
│
│   ├── exec/                # Execution logic, redirections, pipes
│   │   └── exec.c, redir.c, pipe.c
│
│   ├── builtins/            # Built-in command implementations
│   │   └── cd.c, echo.c, env.c, export.c, exit.c, unset.c, pwd.c
│
│   ├── signals/             # Signal handling (Ctrl+C, Ctrl+D, etc.)
│   │   └── signals.c
│
│   └── utils/               # Reusable utility functions
│       └── string_utils.c, error.c, memory.c
│
└── tests/                   # Optional: Shell test scripts
    ├── run_all_test.sh      #executes all listed scripts when 'make debug'
    ├── test_redirects.sh
    └── test_builtins.sh

```

👤 Autor
- Alicia Betancourt ([mainibet](https://github.com/mainibet))
- Joshua Barratt ([jbarrat42](https://github.com/jbarrat42))

