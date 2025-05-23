/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_funs_help.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:46:06 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/23 08:11:54 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_strdup_pars(const char *s1, t_tool *tool)
{
	char	*dup;
	int		len;
	int		i;

	len = 0;
	i = 0;
	while (s1[len])
		len++;
	dup = (char *) malloc(len +1 * sizeof(char));
	if (!dup)
		return (NULL);
	add_to_grbg(&tool->grbg, dup);
	while (i < len)
	{
		dup[i] = s1[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*ft_my_strdup(const char *s1, size_t size, t_tool *tool)
{
	char		*dup;
	size_t		i;

	i = 0;
	dup = (char *) malloc(size + 1 * sizeof(char));
	if (!dup)
		return (NULL);
	add_to_grbg(&tool->grbg, dup);
	while (i < size)
	{
		dup[i] = s1[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

void	add_redir(t_redir **redirs, t_redir *new_redir)
{
	t_redir	*last_redir;

	if (!new_redir)
		return ;
	if (!*redirs)
	{
		*redirs = new_redir;
	}
	else
	{
		last_redir = last_node(*redirs);
		last_redir->next = new_redir;
	}
}
