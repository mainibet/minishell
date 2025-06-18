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

## ⚙️ Usage

### Installation

**Clone the repo**:
```
   git clone https://github.com/yourusername/minishell.git
   cd minishell
```

**Built the project**:
```make```

**Run the shell**
```./minishell```

💡 Supported operations include piping, redirections, environment management, and built-in command execution.

### Dependencies

## 🗂️ File Structure

```
minishell/
├── Makefile                 # Main build script (targets: NAME, all, clean, fclean, re)
├── README.md                # Public documentation: description, installation, usage
├── DEVELOPMENT.md           # Internal notes: features, progress tracking, decisions
│
├── include/                 # Header files
│   └── minishell.h          # Main project header
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
    ├── test_pipeline.sh
    ├── test_redirects.sh
    └── test_builtins.sh

```

## Bash command examples
- what are pipes, how does heredoc work etc.
  
## Limitations
- things to work on in the future

## Resources
- links or books that helped you

## Git

### Commit messages
- git add only the file that you have edited (add file.c)
- commit messages
    - feat -> feature
    - fix -> bug fix
    - docs -> documentation
    - style -> formatting, lint stuff
- commit message example:
    "fix(main.c): fixed x initialization in the main function"
- Git Graph for visualization



👤 Autor

### Action plan
[x] create main.c (Alice)
[ ]


