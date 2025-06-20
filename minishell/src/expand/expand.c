/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 08:56:07 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/20 09:47:21 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	expand_quote(t_expand *expand, char *str, t_tool *tool)
{
	if (expand->flg == 0)
	{
		if (expand->j == 0 || is_space(str[expand->j - 1]))
			expand->before_qoute = 1;
		expand->flg = str[expand->j];
	}
	else if (expand->flg == str[expand->j])
	{
		expand->buff_exp = ft_strjoin(expand->buff_exp,
				ft_strdup("", tool), tool);
		expand->flg = 0;
		expand->before_qoute = 0;
	}
	else
	{
		expand->buff_exp = strjoin_char(expand->buff_exp, str[expand->j], tool);
		expand->is_char = 1;
	}
}

static void	expand_dollar(t_expand *expand, t_tool *tool, char *str, int status)
{
	t_env	*env_node;
	char	*var_name;

	expand->j++;
	if (handle_special(expand, tool, str, status))
		return ;
	var_name = extract_var_name(expand, tool, str);
	if (!var_name)
		return ;
	env_node = search_for_defaults(tool->envh, var_name);
	if (env_node && env_node->value)
		expand_env_variable(expand, tool, env_node);
	else
	{
		if (expand->flg == '"' || expand->is_char == 1)
		{
			expand->buff_exp = ft_strjoin(expand->buff_exp, "", tool);
			expand->is_char = 1;
		}
	}
}

static void	expand_to_buff(t_expand *expand, char *str, t_tool *tool)
{
	if (str[expand->j] == '*' && expand->flg != '\'' && expand->flg != '"')
	{
		expand->buff_exp = strjoin_char(expand->buff_exp, '*', tool);
		expand->j++;
		while (str[expand->j] && str[expand->j] == '*')
			expand->j++;
		expand->is_wildcard = 1;
		if (str[expand->j] != '\0')
			expand->is_char = 1;
		return ;
	}
	expand->buff_exp = strjoin_char(expand->buff_exp, str[expand->j], tool);
	expand->is_char = 1;
	expand->j++;
}

static void	cmd_arg(t_expand *expand, char *cmd_arg, int exit_s, t_tool *tool)
{
	expand->j = 0;
	expand->is_char = 0;
	expand->is_wildcard = 0;
	if (!cmd_arg)
		return ;
	while (cmd_arg[expand->j])
	{
		if (cmd_arg[expand->j] == '"' || cmd_arg[expand->j] == '\'')
		{
			expand_quote(expand, cmd_arg, tool);
			expand->j++;
		}
		else if (cmd_arg[expand->j] == '$' && expand->flg != '\'')
			expand_dollar(expand, tool, cmd_arg, exit_s);
		else
			expand_to_buff(expand, cmd_arg, tool);
	}
}

char	**handel_expand(t_tree *tree, int exit_status, t_tool *tool)
{
	t_expand	expand;
	char		**result;

	if (!tree || !tree->cmd)
		return (NULL);
	init_expand(&expand, tree);
	while (tree->cmd[expand.i])
	{
		cmd_arg(&expand, tree->cmd[expand.i], exit_status, tool);
		if (expand.buff_exp)
		{
			if (expand.is_wildcard)
				process_wildcard_expansion(&expand, tool);
			else
				lst_add_back(&expand.token,
					new_lst(ft_strdup(expand.buff_exp, tool), tool));
			expand.buff_exp = NULL;
			expand.is_char = 0;
			expand.is_wildcard = 0;
		}
		expand.i++;
	}
	result = create_cmd_array_2(expand.token, tool);
	return (result);
}
