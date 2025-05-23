/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:35:16 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/23 08:08:50 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_tree	*parsing_input(char *line, t_tool *tool)
{
	t_token	*token;
	t_tree	*tree;
	int		i;

	tool->quoted = 0;
	tool->anderr = 0;
	tool->paren = 0;
	tool->err = 0;
	tool->grbg = NULL;
	i = 0;
	while (line && line[i])
	{
		hundel_quotes_paren(tool, line[i]);
		i++;
	}
	token = tokens_lst(line, tool);
	if (pars_err(&token, tool))
		return (NULL);
	tree = ft_tree(&token, tool);
	return (tree);
}
