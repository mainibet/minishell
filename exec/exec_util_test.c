// FILE FOR TESTING EXECUTION ONLY"

#include "minishell.h"
#include "exec.h"

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
