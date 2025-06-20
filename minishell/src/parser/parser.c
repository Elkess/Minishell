/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:35:16 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/20 12:19:37 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_struct_tool(t_tool *tool)
{
	tool->quoted = 0;
	tool->anderr = 0;
	tool->paren = 0;
	tool->grbg = NULL;
}

t_tree	*parsing_input(char *line, t_tool *tool)
{
	t_token	*token;
	t_tree	*tree;
	int		i;

	i = 0;
	init_struct_tool(tool);
	while (line && line[i])
	{
		hundel_quotes_paren(tool, line[i]);
		i++;
	}
	token = tokens_lst(line, tool);
	if (pars_err(&token, tool))
		return (NULL);
	tree = ft_tree(&token, tool);
	print_tree(tree);
	return (tree);
}
