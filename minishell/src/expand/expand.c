/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 08:56:07 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/15 10:01:09 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	expand_quote(t_expand *expand, char *str, int status, t_tool *tool)
{
	if (expand->flg == 0)
	{
		if (expand->j == 0 || is_space(str[expand->j - 1]))
			expand->before_qoute = 1;
		expand->flg = str[expand->j];
	}
	else if (expand->flg == str[expand->j])
	{
		expand->buff_exp = ft_strjoin(expand->buff_exp, ft_strdup("", tool), tool);
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
	char	**words;

	env_node = NULL;
	var_name = NULL;
	expand->j++;
	if (str[expand->j] == '?')
	{
		var_name = ft_itoa(status, tool);
		if (var_name)
		{
			lst_add_back(&expand->token, new_lst(ft_strdup(var_name, tool), tool));
			// free(var_name);
		}
		expand->j++;
		return ;
	}
	else if (str[expand->j] == '$')
	{
		expand->buff_exp = strjoin_char(expand->buff_exp, '$', tool);
		expand->is_char = 1;
		expand->j++;
		return ;
	}
	else if (str[expand->j] == '0')
	{
		lst_add_back(&expand->token, new_lst(ft_strdup("minishell", tool), tool));
		expand->j++;
		return ;
	}
	while (str[expand->j] && valid_char(str[expand->j]))
	{
		var_name = strjoin_char(var_name, str[expand->j], tool);
		expand->j++;
	}
	if (!var_name)
		return ;
	env_node = search_for_defaults(tool->envh, var_name);
	if (env_node && env_node->value)
	{
		if (env_node->value && ft_strchr(env_node->value, '*'))
			expand->is_wildcard = 1;
		if (expand->flg != '"' && !expand->is_there_export && has_space(env_node->value))
		{
			words = ft_split(env_node->value, ' ', tool);

			if (has_space(env_node->value) == 2)
            {
                if (expand->buff_exp)
                {
                    lst_add_back(&expand->token, new_lst(ft_strdup(expand->buff_exp, tool), tool));
                    expand->buff_exp = NULL;
                }
                int i = 0;
				while (words[i])
				{
					lst_add_back(&expand->token, new_lst(ft_strdup(words[i], tool), tool));
					i++;
				}
				// free(var_name);
				return ;
            }
			
			if (!expand->buff_exp)
			{
				if (words[0])
					lst_add_back(&expand->token, new_lst(ft_strdup(words[0], tool), tool));
				if (words[1])
					expand->buff_exp = ft_strjoin(expand->buff_exp, words[1], tool);
				else
					expand->buff_exp = NULL;
			}
			else
			{
				if (words[0])
					expand->buff_exp = ft_strjoin(expand->buff_exp, words[0], tool);
				if (expand->buff_exp)
					lst_add_back(&expand->token,
						new_lst(ft_strdup(expand->buff_exp, tool), tool));
				// free(expand->buff_exp);
				if (words[1])
					expand->buff_exp = ft_strjoin(NULL, words[1], tool);
				else
					expand->buff_exp = NULL;
			}
			// free(var_name);
			return ;
		}
		else
			expand->buff_exp = ft_strjoin(expand->buff_exp, env_node->value, tool);
	}
	else
	{
		if (expand->flg == '"' || expand->is_char == 1)
		{
			expand->buff_exp = ft_strjoin(expand->buff_exp, "", tool);
			expand->is_char = 1;
		}
	}
	// free(var_name);
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

char	**handel_expand(t_tree *tree, int exit_status, t_tool *tool)
{
	t_expand	expand;
	char		**result;
	char		**list_wld;
	int			k;

	if (!tree || !tree->cmd)
		return (NULL);
	expand.i = 0;
	expand.j = 0;
	expand.flg = 0;
	expand.is_char = 0;
	expand.is_wildcard = 0;
	expand.token = NULL;
	expand.buff_exp = NULL;
	if (!ft_strcmp(tree->cmd[0], "export"))
		expand.is_there_export = 1;
	else
		expand.is_there_export = 0;
	while (tree->cmd[expand.i])
	{
		expand.j = 0;
		expand.is_char = 0;
		expand.is_wildcard = 0;
		if (!tree->cmd[expand.i])
		{
			expand.i++;
			continue ;
		}
		while (tree->cmd[expand.i][expand.j])
		{
			if (tree->cmd[expand.i][expand.j] == '"'
				|| tree->cmd[expand.i][expand.j] == '\'')
			{
				expand_quote(&expand, tree->cmd[expand.i], exit_status, tool);
				expand.j++;
			}
			else if (tree->cmd[expand.i][expand.j] == '$' && expand.flg != '\'')
				expand_dollar(&expand, tool, tree->cmd[expand.i], exit_status);
			else
				expand_to_buff(&expand, tree->cmd[expand.i], tool);
		}
		if (expand.buff_exp)
		{
			if (expand.is_wildcard)
			{
				list_wld = expand_wildcard(expand.buff_exp, tool);
				k = 0;
				while (list_wld && list_wld[k])
				{
					lst_add_back(&expand.token,
						new_lst(ft_strdup(list_wld[k], tool), tool));
					k++;
				}
			}
			else
				lst_add_back(&expand.token,
					new_lst(ft_strdup(expand.buff_exp, tool), tool));
			// free(expand.buff_exp);
			expand.buff_exp = NULL;
			expand.is_char = 0;
			expand.is_wildcard = 0;
		}
		expand.i++;
	}
	result = create_cmd_array_2(expand.token, tool);
	return (result);
}
