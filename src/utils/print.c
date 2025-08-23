#include "minishell.h"


// Helper: print token list
static void print_tokens_debug(t_token *token)
{
    while (token)
    {
        printf("%s ", token->txt);
        token = token->next;
    }
}

// Recursive AST printer
void print_ast(t_node *node, int level)
{
    if (!node)
        return;

    // Indentation
    for (int i = 0; i < level; i++)
        printf("  ");

    if (node->type == COMMAND)
    {
        printf("COMMAND: ");
        if (node->u_data.cmd.tokens)
            print_tokens_debug(node->u_data.cmd.tokens);
        printf("\n");
    }
    else if (node->type == OPERATOR)
    {
        printf("OPERATOR: ");
        switch (node->u_data.op.type)
        {
            case PIPE: printf("|"); break;
            case AND: printf("&&"); break;
            case OR: printf("||"); break;
            case SEMICOLON: printf(";"); break;
            default: printf("UNKNOWN"); break;
        }
        printf("\n");

        // Recurse into children
        print_ast(node->u_data.op.left, level + 1);
        print_ast(node->u_data.op.right, level + 1);
    }
}
