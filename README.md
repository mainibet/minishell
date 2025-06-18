# minishell
## Description
Minishell is a minimalist UNIX shell written in C that replicates core bash features and 
provides an in-depth exploration of UNIX process management, pipes, signals, file descriptors, and parsing logic

## Key requirements
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

⚙️ Instalación

💡 Uso

✅ Requisitos clave (Key Requirements)

🗂️ Estructura del proyecto ← aquí

👤 Autor

### Action plan
[x] create main.c (Alice)
[ ]

## Usage
- how to compile etc.

## File Structure
- List and explanation of files (e.g., main.c, utils.h, README.md).
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

