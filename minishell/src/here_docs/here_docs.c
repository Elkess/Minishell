/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_docs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:17:26 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/24 11:16:56 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	read_from_pipe(t_redir *red, int status, t_tool *tool, int fd)
{
	char	*lines;
	char	*line;

	while (red && !WIFSIGNALED(status))
	{
		red->content = NULL;
		lines = NULL;
		while (1)
		{
			line = get_next_line(fd);
			if (!line)
				break ;
			lines = ft_strjoin(lines, line, tool);
		}
		red->content = lines;
		red = red->next;
	}
}

void	handle_parent(t_redir *red, t_tool *tool, pid_t pid, int fd[2])
{
	int	status;
	int	sig;

	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			printf("\n");
			tool->herdoc_err = 1;
		}
	}
	close(fd[1]);
	read_from_pipe(red, status, tool, fd[0]);
	close(fd[0]);
	setup_signals();
}

void	here_docs(t_redir *red, t_tool *tool)
{
	int		fds[2];
	size_t	n_herdocs;
	int		pid;

	n_herdocs = there_is_herdoc(red);
	if (n_herdocs)
	{
		while (red)
		{
			if (tool->herdoc_err)
				break ;
			if (red->type == REDIR_HEREDOC)
			{
				fill_fds(fds, red, tool);
				pid = fork();
				if (pid == 0)
					handle_child(red, tool, fds);
				else
					handle_parent(red, tool, pid, fds);
			}
			if (find_lasthd(red) == red)
				break ;
			red = red->next;
		}
	}
}

static char	*expand_quote_file(char *delimiter, t_tool *tool)
{
	char	*result;
	char	quote_char;
	int		i;

	1 && (result = NULL, i = 0, quote_char = 0);
	while (delimiter[i])
	{
		if ((delimiter[i] == '"' || delimiter[i] == '\'') && quote_char == 0)
			quote_char = delimiter[i];
		else if (delimiter[i] == quote_char)
			quote_char = 0;
		else
			result = strjoin_char(result, delimiter[i], tool);
		i++;
	}
	if (!result)
		return (ft_strdup("", tool));
	return (result);
}

void	handle_herdocs(t_tree *tree, t_tool *tool)
{
	t_redir	*red;

	if (!tree)
		return ;
	if (tree && tree->redirs)
	{
		red = tree->redirs;
		while (red)
		{
			if (red->type == 3)
			{
				if (!ft_strchr(red->file, '"') && !ft_strchr(red->file, '\''))
					red->flag = 1;
				red->file = expand_quote_file(red->file, tool);
			}
			red = red->next;
		}
	}
	if (tool->herdoc_err == 1)
		return ;
	here_docs(tree->redirs, tool);
	handle_herdocs(tree->left, tool);
	handle_herdocs(tree->right, tool);
}
