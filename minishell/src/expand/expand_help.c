/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:44:53 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/25 09:57:27 by sgmih            ###   ########.fr       */
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

char	*ext_data_pars(char *line, int flag, t_tool *tool)
{
	size_t	i;

	i = 0;
	while (line[i] && ((line[i] != '+' || line[i + 1] != '=')
			&& line[i] != '='))
		i++;
	if (flag == 1)
	{
		if (!line[i])
			return (NULL);
		if (line[i] == '+' && line[i + 1] == '=')
			return (ft_substr(line, i + 2, ft_strlen(line), tool));
		return (ft_substr(line, ++i, ft_strlen(line), tool));
	}
	return (ft_substr(line, 0, i, tool));
}

void	check_export_keys(t_expand *expand, t_tree *tree, t_tool *tool)
{
	char	*key;
	char	*val;
	int		j;

	key = NULL;
	val = NULL;
	expand->flg_split = 0;
	expand->val_quotes = 0;
	j = 1;
	if (!ft_strcmp(tree->cmd[0], "export"))
		expand->is_there_export = 1;
	else
		expand->is_there_export = 0;
	while (tree->cmd[j])
	{
		key = ext_data_pars(tree->cmd[j], 0, tool);
		val = ext_data_pars(tree->cmd[j], 1, tool);
		if (is_valid_key(key))
			expand->flg_split = 1;
		check_if_quoted(val, expand);
		j++;
	}
}

void	init_expand(t_expand *expand, t_tree *tree, t_tool *tool)
{
	expand->i = 0;
	expand->j = 0;
	expand->flg = 0;
	expand->is_char = 0;
	expand->is_wildcard = 0;
	expand->token = NULL;
	expand->buff_exp = NULL;
	check_export_keys(expand, tree, tool);
	check_export_split(expand);
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
