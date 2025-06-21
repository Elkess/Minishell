/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_docs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:17:26 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/21 08:00:18 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	read_from_pipe(t_redir *red, int status, t_tool *tool, int fd)
{
	char	*lines;
	char	buff[2];

	while (red && !WIFSIGNALED(status))
	{
		if (red->type == REDIR_HEREDOC)
		{
			red->content = NULL;
			lines = NULL;
			while (1)
			{
				if (read(fd, buff, 1) <= 0)
					break ;
				if (buff[0] == '\0')
					break ;
				buff[1] = '\0';
				lines = ft_strjoin(lines, buff, tool);
			}
			red->content = lines;
		}
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
	int		fd[2];
	size_t	n_herdocs;
	pid_t	pid;

	if (pipe(fd) == -1)
		print_err(NULL, "pipe failed :", strerror(errno));
	n_herdocs = there_is_herdoc(red);
	pid = 0;
	if (n_herdocs)
		pid = fork();
	if (pid == 0 && n_herdocs)
		handle_child(red, tool, fd);
	handle_parent(red, tool, pid, fd);
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
		{
			quote_char = delimiter[i];
			i++;
			if (delimiter[i + 1] == '\0')
				return (ft_strdup("", tool));
		}
		else if (delimiter[i] == quote_char)
		{
			quote_char = 0;
			i++;
		}
		else
			result = strjoin_char(result, delimiter[i++], tool);
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
