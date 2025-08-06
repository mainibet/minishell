
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
 * single token.
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
 *   * SEMICOLON.
 *   operator nodes carry only the type.
 * - an atomic command (terminal): commands can be external or built-in, and are handled
 *   by `exec_terminal`.  terminal nodes carry a token list
 * all operators are left-associative except pipe, which is right-associative in order 
 * to limit the number of simultaneously open file descriptors to three
 */
t_node *parse(t_token *token_list);

/* @brief traverse a tree left to right, depth first, calling exec_terminal for each 
 * terminal node
 * @param tree the root node of the parse tree
 * @param env the current environment
 * for each terminal node, exec_terminal with the token list and all open file 
 * descriptors.
 * - fd[0] is the terminal node's input fd.  it may be 0 or the write side fd of the 
 *   previous pipe.
 * - fd[1] is the terminal node's output fd.  it may be 1 or the read side fd of the 
 *   succeeding pipe
 * - fd[3] is irrelevant to the terminal node's execution but must be closed in the child
 *   process if the token list indicates an external program.  it may be -1 
 *   (if the terminal is on the RHS of the last operator, 0 if the succeeding operator
 *   is not a pipe, or the write side fd of the succeeding pipe.
 */
int	traverse(t_node *tree, char **env);

/* @brief execute an atomic command given by tokens with the input and output streams 
 * in fd
 * @param tokens a token list with all the tokens in the atomic command.
 * @param fd the input, output and extra file descriptors
 * determine whether an atomic command is a built-in or external program and execute 
 * it.
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
 * a subsequent fork.
 */
int exec_terminal(t_token *tokens, int fd[3], char **env);
