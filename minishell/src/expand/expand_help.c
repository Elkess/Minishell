/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:44:53 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/21 15:18:25 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_question(t_expand *expand, t_tool *tool, int status)
{
	char	*var_name;

	var_name = ft_itoa(status, tool);
	if (var_name)
		expand->buff_exp = ft_strjoin(expand->buff_exp, var_name, tool);
	expand->j++;
	return (1);
}

void	init_expand(t_expand *expand, t_tree *tree)
{
	expand->i = 0;
	expand->j = 0;
	expand->flg = 0;
	expand->is_char = 0;
	expand->is_wildcard = 0;
	expand->token = NULL;
	expand->buff_exp = NULL;
	if (!ft_strcmp(tree->cmd[0], "export"))
		expand->is_there_export = 1;
	else
		expand->is_there_export = 0;
}

void	process_wildcard_expansion(t_expand *expand, t_tool *tool)
{
	char	**list_wld;
	int		k;

	list_wld = expand_wildcard(expand->buff_exp, tool);
	k = 0;
	while (list_wld && list_wld[k])
	{
		lst_add_back(&expand->token,
			new_lst(ft_strdup(list_wld[k], tool), tool));
		k++;
	}
}
