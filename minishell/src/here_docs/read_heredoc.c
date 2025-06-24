/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:34:19 by melkess           #+#    #+#             */
/*   Updated: 2025/06/24 20:17:15 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	fill_fds(int *fds, t_redir *red, t_tool *tool)
{
	char	*str;

	str = generate_file(red, tool);
	fds[0] = open(str, O_CREAT | O_RDONLY);
	if (fds[0] < 0)
		return (tool->herdoc_err = 1,
			print_err(NULL, "fds failed", strerror(errno)));
	fds[1] = open(str, O_CREAT | O_WRONLY);
	if (fds[1] < 0)
		return (tool->herdoc_err = 1,
			print_err(NULL, "fds failed", strerror(errno)));
	unlink(str);
}

char	*generate_file(t_redir *red, t_tool *tool)
{
	char	*str;
	size_t	i;

	str = NULL;
	i = 0;
	while (1)
	{
		str = ft_strjoin("/tmp/.here_doc", ft_itoa(i, tool), tool);
		red->fd = open(str, O_CREAT | O_RDWR | O_APPEND | O_EXCL, 0644);
		if (red->fd != -1)
			break ;
		i++;
	}
	return (str);
}

void	write_helper(char *file, t_redir *red, t_tool *tool, int status)
{
	char	*line;
	int		fd;

	fd = open(file, O_CREAT | O_RDWR, 0644);
	if (fd == -1)
		(perror("Open failed"));
	line = get_next_line(red->fd);
	while (line)
	{
		add_to_grbg(&tool->grbg, line);
		line = expand_herdoc_content(line, tool, status);
		write(fd, line, ft_strlen(line));
		line = get_next_line(red->fd);
	}
	(close(red->fd), close(fd));
	red->fd = open(file, O_CREAT | O_RDWR, 0644);
	if (red->fd == -1)
		(perror("Open failed"));
	unlink(file);
}

void	read_from_heredoc(t_redir *red, t_tool *tool, int status)
{
	char	*file;

	while (red)
	{
		if (red->type == REDIR_HEREDOC)
		{
			file = generate_file(red, tool);
			write(red->fd, red->content, ft_strlen(red->content));
			close(red->fd);
			red->fd = open(file, O_CREAT | O_RDWR, 0644);
			if (red->fd == -1)
				(perror("Open failed"));
			unlink(file);
			if (red->flag)
				write_helper(file, red, tool, status);
		}
		red = red->next;
	}
}

int	hlp(const char *str, int i, t_expand *exp, t_tool *tool)
{
	int		start;
	char	*word;

	start = i;
	while (str[i] && str[i] != ' ')
		i++;
	if (i > start)
	{
		word = ft_substr(str, start, i - start, tool);
		lst_add_back(&exp->token, new_lst(word, tool));
	}
	return (i);
}
