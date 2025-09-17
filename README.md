# minishell
## Description
Minishell is a minimalist UNIX shell written in C that replicates core bash features and 
provides an in-depth exploration of UNIX process management, pipes, signals, file descriptors, and parsing logic

## ✅ Key requirements
- Display a prompt when waiting for a new command.
- Have a working history.
- Search and launch the right executable.
- Use at most one global variable to indicate a received signal.

## 📚 Allowed Functions

The project must be implemented **only** using the following functions:

- `readline`, `rl_clear_history`, `rl_on_new_line`, `rl_replace_line`, `rl_redisplay`, `add_history`
- `printf`, `malloc`, `free`, `write`
- `access`, `open`, `read`, `close`
- `fork`, `wait`, `waitpid`, `wait3`, `wait4`
- `signal`, `sigaction`, `sigemptyset`, `sigaddset`, `kill`
- `exit`, `getcwd`, `chdir`
- `stat`, `lstat`, `fstat`, `unlink`
- `execve`, `dup`, `dup2`, `pipe`
- `opendir`, `readdir`, `closedir`
- `strerror`, `perror`
- `isatty`, `ttyname`, `ttyslot`, `ioctl`
- `getenv`, `tcsetattr`, `tcgetattr`
- `tgetent`, `tgetflag`, `tgetnum`, `tgetstr`, `tgoto`, `tputs`

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

## 💡 Usage

Supported operations include piping, redirections, environment management, and built-in command execution.

Is not handled || ; && $$

### Dependencies
To compile and run **Milanshell**, you need:

- [GNU Readline](https://tiswww.case.edu/php/chet/readline/rltop.html) library  
  - Ubuntu/Debian: `sudo apt-get install libreadline-dev`  
  - macOS: `brew install readline`

- C compiler (e.g. `gcc` or `clang`)
- `make` utility
- libft (already included in this repo / add it as submodule if not present)

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

## 🧱 Input Processing Flow
1. **Lexing** – Takes the full command line and splits it into logical units called tokens. For example, the input "echo "hello world" | wc -w" becomes a token list like [echo], [hello world], [|], [wc], [-w]. This lexer handles quotes and separates words based on spaces and operators.

2. **Parser** – After the lexer, the parser organizes the token list into a hierarchical structure called an Abstract Syntax Tree (AST). This tree represents the logical structure of the command line, respecting the precedence of operators (|, &&, ||, ;). This allows us to understand which commands to run and in what order. For instance, in a command with pipes, the tree will have a | operator node with a command on its left and another on its right.

3. **Prexecution** - In this phase, the AST is prepared for execution. Environment variables (like $PATH or $HOME) are expanded, I/O redirections and heredocs are managed, and absolute paths for commands are resolved.

3.1. **Expansion** – Environment variables (`$VAR`, `$?`) are expanded, respecting shell quoting rules:
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

## 🧪 Testing

You can test **Milanshell** in different ways:

- **Basic commands**
  ```bash
  `echo hello`
  `ls -l`
  `cd ..`
  `echo hi | grep h`
  `echo -n hello | cat -e`
  `echo hi > out.txt | cat out.txt`
  `cat | cat | ls`
  `cat << EOF`
  `grep "hello"`
  `|`
  `<<'
  `ls | ls`
  `echo hello > file1 > file2 > file3`

- Compare with Bash
Run the same commands in bash and ./minishell → results should match.

- Check for memory leaks

We ran extensive tests for parsing, redirections, pipelines, signals, and builtins.  
✅ See some more cases **Test Cases and Results** [here](docs/test_cases.md).

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
│   ├── main.c main_util.c
│
│   ├── lexer/               # Lexical analysis: tokenization and quote preservation
│   │   └── lexer.c lex_quote.c lex_token.c lex_word.c
│
│   ├── parser/              # Parses tokens into commands, handles syntax & structure
│   │   └── parser.c parse_node.c parse_cmd_util.c parser_arg.c parser_cmd.c parser_token.c
│
│   ├── exec/                # Execution logic, redirections, pipes, etc
│   │   └── exec.c, redir.c, pipe.c
│   │
│   ├── builtins/            # Built-in command implementations
│   │   └── cd.c, cd_util.c echo.c, env.c, env_util.c export.c, exit.c, export_util.c export_util_01.c export_util_02.c export_util_03.c unset.c, pwd.c
│   │
│   ├── expansion/
│   │   └── exp.c, exp_util.c heredoc.c heredoc_init.c heredoc_io.c heredoc_util.c
│   │
│   ├── signals/             # Signal handling (Ctrl+C, Ctrl+D, etc.)
│   │   └── signal.c signal_util.c
│
│   └── utils/               # Helper functions
│       └── cleanup.c cleanup_fd.c cleanup_node.c cleanup_token.c init.c print.c
│
└── tests/                   # Optional: Shell test scripts or CI/CD when push
    ├── run_all_test.sh      #executes all listed scripts when 'make debug'
    ├── test_redirects.sh (example)
    └── test_builtins.sh (example)

## Additional Documentation
- [Test Cases and Results](https://github.com/mainibet/minishell/blob/main/docs/TEST.md)
- [AST Structure Example](https://github.com/mainibet/minishell/blob/main/docs/AST_EXAMPLE.md)


```

👤 Autor
- Alicia Betancourt ([mainibet](https://github.com/mainibet))
- Tanmay Pandya ([mainibet](https://github.com/tpandya42))

