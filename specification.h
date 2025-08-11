/* @brief read one line (and maybe a heredoc) from the terminal, check for validity, 
 * and update history.
 * @return a string containing one line and possibly a heredoc on subsequent lines
 * use `readline()` to read a line of commands from the terminal.  if `has_heredoc()`
 * keep reading further lines until the heredoc delimiter is found on its own line.
 * NOTE: the return string is malloc'd and thus must ultimately be freed.
 */
char *shell_get_command();

/* @brief take an input string and return another string with the variables expanded
 * @param s the input string
 * @param last_status the return status of the last command (to expand $?)
 * @param argv the input arguments of the shell itself (to expand $0, $1, etc.)
 * takes s, calculates how long it would be if all the variables were expanded,
 * allocates a new string, copies s into it with the variables expanded, frees s
 * and returns the new string.  
 * NOTE: variables inside single quotes are not expanded.
 */
char *expand_variables(char *s, int last_ret, char **argv);

/* @brief expand variables and tokenize an input string
 * @param s the input string, with all variables expanded
 * @param delim the token delimiter.  this should be ' ' unless called recursively
 * @param env the current environment of the shell.
 * create a linked list of `t_token`s from s.  quoted substrings are returned as a 
 * single token.  Heredoc tokens should be so typed and their text replaced by the 
 * text of the heredoc.
 * MAYBE: type file redirections as such and replace text with file path?
 */
t_token	*lex(char *s, char delim, char **env);

/* @brief create a parse tree from a token list
 * @param token_list linked list of tokens with variables expanded
 * NOTE: this might break up the token list so it is no longer freeable; in this case
 * sublists should be freed when nodes are freed.
 * each node in the tree is either an
 * - operator (non-terminal): operators can be, in decreasing order of precedence 
 *   * PIPE, 
 *   * AND/OR, or 
 *   * SEMICOLON
 *   operator nodes carry only the type.
 * - an atomic command (terminal): commands can be external or built-in, and are handled
 *   by `exec_terminal`.  terminal nodes carry a token list
 * all operators are left-associative except pipe, which is right-associative in order
 * to limit the number of simultaneously open file descriptors to three
 */

t_node *parse(t_token *token_list);

/* @brief traverse a tree left to right, depth first, calling exec_terminal on each
 * terminal leaf
 * @param tree the root node of the parse tree
 * @param env the current environment
 * If tree->type is OPERATOR:
 * If tree->data.type is PIPE:
 * - recurse on tree->data.left then tree->data.right.
 * - if tree->data.right->type == TERMINAL, execute wait_child() on pid_right;
 * - execute wait_child() on pid_left
 * - return the value of wait_child(pid_right)
 * If tree->data.type is not PIPE:
 * - recurse on tree->data.left. if tree->data.left->type is TERMINAL execute wait_child
 *   on it.
 *   - if tree->data.type is AND and wait_pid(pid_left) is not 0, 
 *     return that value.
 *   - if tree->data.type is OR and wait_child(pid_left) is 0, return
 *     that value.
 * - recurse on tree->data.right. if tree->data.right->type is TERMINAL execute wait_child
 *   on it.  Return the value of wait_child(pid_right)
 * If tree->type is TERMINAL, run exec_terminal on tree->data.tokens and all open file 
 * descriptors.
 * - fd[0] is the terminal node's input fd.  it may be 0 or the write side fd of the 
 *   previous pipe.
 * - fd[1] is the terminal node's output fd.  it may be 1 or the read side fd of the 
 *   succeeding pipe
 * - fd[3] is irrelevant to the terminal node's execution but must be closed in the child
 *   process if the token list indicates an external program.  it may be -1 
 *   if the terminal is on the RHS of the last operator, 0 if the succeeding 
 *   operator is not a pipe, or the write side fd of the succeeding pipe.
 */
int	traverse(t_node *tree, char **env, pid_t *pid, t_alloced *alloced);

/* @brief execute an atomic command given by tokens with the input and output streams 
 * in fd
 * @param a node of type TERMINAL containing a token list and a pid placeholder
 * @param fd the input, output and extra fds
 * determine whether an atomic command is a built-in or external program and execute 
 * it.  If it is an external command, set the pid returned by fork()
 * The token list may contain redirects, which will cause file descriptors to be 
 * replaced:
 * - `<file` requires us to close fd[0] and replace it with the file descriptor 
 *   returned by open(file, O_RDONLY)
 * - `>file` requires us to close fd[1] and replace if with the file descriptor 
 *   returned by open(file, O_WRONLY)
 * - `>>file` requires us to close fd[1] and replace it with the file descriptor
 *   returned by open(file, O_WRONLY & O_APPEND)
 * - `<<EOF` requires us to close fd[0], create a pipe, write the remainder of the token
 *   (heredocs should probably be just one token) to the read side of the pipe and 
 *   assign the write side of the pipe to fd[0]
 * fd[2] is irrelevant to the execution, but should be closed by any child resulting from
 * a subsequent fork.  The child must also free all allocated memory.
 */
void exec_terminal(t_node node, int fd[3], t_context *context);

/* @brief wait for the child process pid to end and interpret the w_status value, 
 * returning the return status of the child process.
 */
int wait_child(int pid);

/* ================= EXECUTION PART ================= */

/* @brief process a node: prepare and execute commands 
* (to be called from main after parsing)
 * @param program the program context
 * @return the return status of the last command
 */
int process_node(t_context *program);

/* @brief execute the command tree
 * @param program the program context
 * @param node the root of the command tree
 * @return the return status of the last command
 */
int execution(t_context *program, t_node *node);

/* @brief check if a command is a builtin and execute it */
int is_builtin(const char *cmd_name);
int execute_builtin(t_node *node);

/* @brief builtin implementations */
int my_echo(t_node *node);
int my_env(char **envp);
int my_export(char **envp);
int my_pwd(char *cwd_path);

/* @brief string comparison (used by builtins) */
int ft_strcmp(const char *s1, const char *s2);

/* @brief find and resolve executable path */
char *find_path(char *argv);

/* @brief convert tokens to argv and pre-execution setup */
char **token_to_argv(t_token *token);
void pre_execution(t_node *node, char **envp);

/* @brief setup input/output redirections */
int redir_input(int fd);
int redir_output(int fd);
int setup_redir(int fd_in, int fd_out, t_fd_dup *dup);

/* @brief execute a command node in a child process */
void execute_in_child(t_node *node);

/* @brief execute a simple command or external command */
int execute_simple_cmd(t_node *cmd_node);
void exec_external_cmd(t_node *node);

/* @brief handle operators in the execution tree */
static int handle_operator(t_node *node);

/* @brief handle command execution in parent process */
int handle_cmd_exec(t_node *node, bool is_pipe_child);
int execute_cmd(t_node *node);

/* @brief execute left/right side of a pipe and pipeline */
pid_t execute_left(t_node *left_node, int pipefd[2]);
pid_t execute_right(t_node *right_node, int pipefd[2]);
int execute_pipeline(t_node *node);

/* @brief wait for left/right child processes in a pipeline */
int wait_children(pid_t left_pid, pid_t right_pid, int *right_status);

/* @brief cleanup resources for a command node */
int cleanup_cmd_node(t_node *node);
int cleanup_operator_fd(t_node *node);
int cleanup_fd(t_node *node, t_cmdtype type);
