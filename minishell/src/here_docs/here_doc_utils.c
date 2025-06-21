/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:37:47 by melkess           #+#    #+#             */
/*   Updated: 2025/06/21 15:24:39 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	count_heredocs(t_tree *tree)
{
	int		count;
	t_redir	*red;

	count = 0;
	if (!tree)
		return (0);
	red = tree->redirs;
	while (red)
	{
		if (red->type == REDIR_HEREDOC)
			count++;
		red = red->next;
	}
	count += count_heredocs(tree->left);
	count += count_heredocs(tree->right);
	return (count);
}

size_t	there_is_herdoc(t_redir *red)
{
	size_t	i;

	i = 0;
	while (red)
	{
		if (red->type == REDIR_HEREDOC)
			i++;
		red = red->next;
	}
	return (i);
}

t_redir	*find_lasthd(t_redir *redirs)
{
	t_redir	*last;

	last = NULL;
	while (redirs)
	{
		if (redirs->type == REDIR_HEREDOC)
			last = redirs;
		redirs = redirs->next;
	}
	return (last);
}

void	here_doc(t_tool *tool, t_redir *red, int fd)
{
	struct termios	orig_termios;
	char			*line;
	char			*lines;

	line = NULL;
	lines = NULL;
	while (1)
	{
		disable_echoctl(&orig_termios);
		line = readline("> ");
		if (line)
			add_to_grbg(&tool->grbg, line);
		if (!line || !ft_strcmp(line, red->file))
		{
			if (!lines)
				write(fd, "\0", 1);
			break ;
		}
		line = ft_strjoin(line, "\n", tool);
		lines = ft_strjoin(lines, line, tool);
	}
	if (lines)
		write(fd, lines, ft_strlen(lines) + 1);
}

void	handle_child(t_redir *red, t_tool *tool, int fd[2])
{
	close(fd[0]);
	signal(SIGINT, SIG_DFL);
	while (red)
	{
		if (red->type == REDIR_HEREDOC)
			here_doc(tool, red, fd[1]);
		if (find_lasthd(red) == red)
			exit(0);
		red = red->next;
	}
	exit(0);
}
