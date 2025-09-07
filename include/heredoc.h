/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 16:37:46 by albetanc          #+#    #+#             */
/*   Updated: 2025/09/07 17:28:02 by albetanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include "minishell.h"

typedef struct s_heredoc
{
	t_redir		*redir;
	t_program	*program;
	char		*buf;
	int			pipefd[2];
	int			current_line;
	char		*delimiter;
}	t_heredoc;

int		heredoc_prepare(t_redir *redir, t_program *program);
void	heredoc_normalize_delimiter(t_redir *redir);
int		is_quoted(const char *s);
char	*strip_outer_quotes(const char *s);

#endif
