/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_docs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:17:26 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/15 14:31:01 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*expand_quote_file(char *delimiter, t_tool *tool)
{
	char	*result;
	char	quote_char;
	int		i;

	result = NULL;
	i = 0;
	quote_char = 0;
	while (delimiter[i])
	{
		if ((delimiter[i] == '"' || delimiter[i] == '\'') && quote_char == 0)
		{
			quote_char = delimiter[i];
			i++;
		}
		else if (delimiter[i] == quote_char)
		{
			quote_char = 0;
			i++;
		}
		else
			result = strjoin_char(result, delimiter[i++], tool);
	}
	return (result);
}

void	handle_herdocs(t_tree *tree, t_env *envh, t_tool *tool)
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
	here_docs(tree->redirs, envh, tool);
	handle_herdocs(tree->left, envh, tool);
	handle_herdocs(tree->right, envh, tool);
}
