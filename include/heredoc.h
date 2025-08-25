#ifndef HEREDOC_H
#define HEREDOC_H

# include "minishell.h"

int	heredoc_prepare(t_redir *redir, char **envp_cpy, int last_exit);
void	heredoc_normalize_delimiter(t_redir *redir);

#endif
