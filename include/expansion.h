#ifndef EXPANSION_H
# define EXPANSION_H
# include "minishell.h"

char	*get_env_value(const char *var, char **envp_cpy);
char	*status_exp(char c, int last_exit);
char	*expand_token_helper_01(char **pos, char **envp_cpy);
char	*expand_token_helper(char *result, char *pos);
void	expand(t_token *token, char **envp_cpy, int last_exit);
char	*expand_token_text(char *txt, char **envp_cpy, int last_exit);


#endif
