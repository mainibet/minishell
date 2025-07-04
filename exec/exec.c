/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:32:13 by albetanc          #+#    #+#             */
/*   Updated: 2025/07/04 12:11:52 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

 #include "minishell.h" 
 #include "exec.h"//temporary for testing

//---------------------------------------//
//V0:Execute single commands directly,   //
//   from readline                       //
//                                       //
//V1:supports basic parsing              //
//                                       //
//V2:support parsing refinement          //
//                                       //
//V3:execution with pipes & child pcs    //
//                                       //
//V4: support all built-ins              //
//                                       //
//V5: support heredoc & redirections     //
//---------------------------------------//

// Read a full line as raw input.
// Use fork() and execvp() to execute the input line as-is.
// Wait for the child process (waitpid).
// Exit command ends the shell.
// Should check for builtins commands if not, look for external commands ($PATH)

// need to duplicate the arguments (char **argv) before passing them to the child process 
//duplicating the arguments gives the child process its own isolated and safe copy of the command-line arguments. 
//  This prevents unintended side effects and memory issues between the parent and child processes, 
//  particularly crucial when execve() is about to replace the child's entire memory space.

//VALORACION TMP PARA EMPEZAR
//  For each external command in that list, you will:
// Fork a new child process.
// Set up redirections (using adapted setup_redir and redir_input/output).
// Execute the command using execve (similar to your execution function).
// In the parent, wait for that child (using wait_child). This generic, looped approach is what allows Minishell to handle N commands.

//Initially I received a string with the cmd, flags and args
//With the tree I tree node and envp
// void	execution(char	**nargv, char **const envp)
//V0 just executes single cmd
//WHat tokenzation did: char *argv[] = {"ls", "-l", "/home", NULL};
//argv[0] is command name

//-------------------------------//
//	HELPER FUNCTION FOR TESTING  //
//-------------------------------//

/*
    Duplicates a single string
    Return new str or NULL if error
    Memory: allocated with strdup
*/
char *duplicate_string(const char *s)
{
    char *new_s;

    if (!s)
        return (NULL);
    new_s = strdup(s);
    if (!new_s) {
        perror("strdup failed in duplicate_string");
        return (NULL);
    }
    return (new_s);
}

/*
   Return array or NULL if error
   1. Count # tokens in original array
   2. Get memory for new arr(duplicated_array)
   3. Dup each stirng inside
*/
char **duplicate_tokens(char **original_tokens)
{
    char    **duplicated_array;
    int     count;
    int     i;

    if (!original_tokens)
        return (NULL);
    count = 0;
    while (original_tokens[count] != NULL)
        count++;
    duplicated_array = (char **)malloc(sizeof(char *) * (count + 1));
    if (!duplicated_array) {
        perror("malloc for duplicated_array failed in duplicate_tokens");
        return (NULL);
    }
    i = 0;
    while (i < count) {
        duplicated_array[i] = duplicate_string(original_tokens[i]);
        if (!duplicated_array[i]) 	// Check for memory allocation failure
		{
			free_array(duplicated_array, i);// Free previously allocated strings
			perror("strdup failed in duplicate_tokens");
			return (NULL);
		}
        i++;
    }
    duplicated_array[count] = NULL;//end correctly the array
    return (duplicated_array);
}

// --- Función para llenar el argv de un nodo de comando (usando duplicate_tokens) ---
// Retorna 0 en caso de éxito, -1 en caso de fallo (por malloc/strdup)
int fill_command_node(t_node *node, char **tokens)
{
    if (!node || node->type != NODE_CMD || !tokens) 
    {
        fprintf(stderr, "Error: Invalid arguments to fill_command_node_argv.\n");
        return (-1);
    }
    node->argv = duplicate_tokens(tokens);
    if (!node->argv)
        return (-1);//ERROR
    return (0); // SUCCESS
}

// --- CREATE AND INITIALIZE A NEW NODE --- //
t_node *create_node(t_node_type type) 
{
    t_node *new_node;
    
    new_node = (t_node *)malloc(sizeof(t_node));
    if (!new_node) 
	{
        perror("malloc failed creating new node");
        exit(EXIT_FAILURE); //in stdlib.h
	}
    new_node->type = type;
    new_node->argv = NULL;
    new_node->filename = NULL;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

// -- EXIT ERROR CLEAN-UP -- //

void free_node(t_node *node)
{
    if (!node)
        return;
    if (node->argv) 
    {
        free_array(node->argv, 0);
        node->argv = NULL;
    }
    if (node->filename) 
    {
        free(node->filename);
        node->filename = NULL;
    }
    free(node);
}

void exit_error_program(const char *msg, char *line_to_free, char **tokens_to_free, t_node *node_to_free)
{
    perror(msg);
    if (line_to_free)
        free(line_to_free);
    if (tokens_to_free)
        free(tokens_to_free);
    if (node_to_free)
        free_node(node_to_free);
    exit(EXIT_FAILURE);
}

/*
    Count the number of tokens in a string using strtok.
    Returns the number of tokens found or 0 if the input string is NULL.
    Memory: allocated with strdup
*/
int count_tokens(const char *s, const char *delimiter) //JUST TESTING
{
    char *temp_s;
    char *token;
    int count = 0;

    if (!s) return 0;
    temp_s = strdup(s);
    if (!temp_s) 
    {
        perror("strdup falló en count_tokens");
        return 0;
    }
    token = strtok(temp_s, delimiter);//STRTOK NOT ALLOWED
    while (token != NULL) 
    {
        count++;
        token = strtok(NULL, delimiter);//STRTOK NOT ALLOWED
    }
    free(temp_s);
    return count;
}

char **parse_line_to_tokens(char *line, const char *delimiter) {
    char **tokens_array;
    char *token;
    int token_count;
    int i;

    if (!line || *line == '\0') 
        return (NULL);
    token_count = count_tokens(line, delimiter);
    if (token_count == 0) 
        return (NULL);
    tokens_array = (char **)malloc(sizeof(char *) * (token_count + 1));
    if (!tokens_array) 
    {
        perror("malloc para tokens_array falló en parse_line_to_tokens");
        return (NULL);
    }
    token = strtok(line, delimiter);
    i = 0;
    while (token != NULL) {
        tokens_array[i] = token;
        i++;
        token = strtok(NULL, delimiter);
    }
    tokens_array[i] = NULL;
    return (tokens_array);
}

//-------------------------------//
//         EXECUTION   		     //
//-------------------------------//
void	execution (t_node *root, char **envp)
{
	char	*cmd_path;

	cmd_path = find_path (root->argv[0]);//I will received this from parsing
	if (!cmd_path)//this needs to be done in the parsing
	{
		perror ("command_path not found");//TODO: include general celan-up and check were to call t for free root_node
		exit(EXIT_FAILURE);
	}
	execve(cmd_path, root->argv, envp);
	perror ("execve failed");
	free (cmd_path);//freed here 'cause was created here and used here
	//TODO: include general celan-up and check were to call t for free root_node
	exit (EXIT_FAILURE);//later in the program a general celan-up to free all nodes
}

/*
   v0: display prompt and read a line from stdin
   v1: tokenize the line into commands and arguments
   v2: parse the tokens into a tree structure
   v3: execute the commands in the tree structure
*/

// -----------------------------//
//       MAIN FOR TESTING 		//
// -----------------------------//


/*
    1. Reads a line from stdin
    2. Set the prompt
    3. It uses readline for input and adds history support.
    4. tokenizes args from cmd line
    5. Creates a command node,
    6. Fills the command node with arguments
    7. Executes the command.
    
*/

/*
  next steps:
   - Implement 1 fork for execute 1 command. GOAL: keep alife minishell
   
*/
int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	*prompt;
	t_node	*root;//TEST
    char    **parsed_tokens;//TEST

	(void)argc; // TEST
	(void)argv; // TEST
	prompt = GREEN BOLD "🐶🥕 Milanshell >" RESET;
	while (1)
	{
		line = readline(prompt);
		if (line == NULL)
		{	
			printf("exit\n");
			break ;
		}
		if (*line)
			add_history(line);
		printf("Command received: %s\n", line);
		parsed_tokens = parse_line_to_tokens(line, " "); //TEST
        if (parsed_tokens == NULL) //TEST
        {
            free(line); 
            continue; //CHECK IF CAN BE USED and how it works
        }
		root = create_node(NODE_CMD); 
		if (!root)//TEST
		    exit_error_program("Failed to create command node", line, NULL, NULL);//TEST
        if (fill_command_node(root, parsed_tokens) == -1)//TEST 
            exit_error_program("Failed to fill command node argv", line, NULL, root);//TEST
		execution(root, envp);//TEST
        free(parsed_tokens);
		free(line);
	}
	// TODO clean-up all nodes and free memory
	return (0);
}
