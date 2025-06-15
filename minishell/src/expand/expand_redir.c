/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:55:36 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/15 14:42:16 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_file_expansion(t_redir *red, char **expand_file, t_tool *tool)
{
	int	file_count;

	if (!expand_file)
	{
		red->is_ambiguous = 1;
		return ;
	}
	file_count = 0;
	while (expand_file[file_count])
		file_count++;
	if (file_count == 1)
	{
		red->file = ft_strdup(expand_file[0], tool);
		red->is_ambiguous = 0;
	}
	else
		red->is_ambiguous = 1;
}

void	expand_redir(t_tree *tree, t_tool *tool, int status)
{
	t_redir	*red;
	t_tree	temp_tree;
	char	**expand_file;
	char	*temp_cmd[2];

	red = tree->redirs;
	while (red)
	{
		if (red->type == 0 || red->type == 1 || red->type == 2)
		{
			temp_cmd[0] = red->file;
			temp_cmd[1] = NULL;
			temp_tree.cmd = temp_cmd;
			expand_file = handel_expand(&temp_tree, status, tool);
			handle_file_expansion(red, expand_file, tool);
		}
		red = red->next;
	}
}
