/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:40:55 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/24 18:27:59 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_special(t_expand *exp, t_tool *tool, char *str, int status)
{
	if (str[exp->j] == '?')
		return (handle_question(exp, tool, status));
	else if (str[exp->j] == '$')
	{
		exp->buff_exp = strjoin_char(exp->buff_exp, '$', tool);
		exp->is_char = 1;
		exp->j++;
		return (1);
	}
	else if (str[exp->j] == '0')
	{
		exp->buff_exp = ft_strjoin(exp->buff_exp,
				ft_strdup("minishell", tool), tool);
		exp->j++;
		return (1);
	}
	else if (str[exp->j] == '\0')
	{
		exp->buff_exp = strjoin_char(exp->buff_exp, '$', tool);
		exp->is_char = 1;
		return (1);
	}
	return (0);
}

char	*extract_var_name(t_expand *expand, t_tool *tool, char *str)
{
	char	*var_name;

	var_name = NULL;
	while (str[expand->j] && valid_char(str[expand->j]))
	{
		var_name = strjoin_char(var_name, str[expand->j], tool);
		expand->j++;
	}
	return (var_name);
}

static void	handle_space_expansion(t_expand *expand, t_tool *tool, char **words)
{
	if (!expand->buff_exp)
	{
		if (words[0])
			lst_add_back(&expand->token,
				new_lst(ft_strdup(words[0], tool), tool));
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
		if (words[1])
			expand->buff_exp = ft_strjoin(NULL, words[1], tool);
		else
			expand->buff_exp = NULL;
	}
	return ;
}

void	expand_env_variable(t_expand *exp, t_tool *tool, t_env *env_node)
{
	char	**words;

	if (exp->flg != '"' && !exp->is_there_export && has_space(env_node->value))
	{
		words = ft_split(env_node->value, ' ', tool);
		if (!words || !words[0])
			return ;
		handle_space_expansion(exp, tool, words);
	}
	else
	{
		exp->buff_exp = ft_strjoin(exp->buff_exp, env_node->value, tool);
	}
}

void	expand_env_with_split(t_expand *exp, t_tool *tool, t_env *env_node)
{
	int		k;

	k = 0;
	while (env_node->value[k] && env_node->value[k] == ' ')
		k++;
	while (env_node->value[k] && env_node->value[k] != ' ')
	{
		exp->buff_exp = strjoin_char(exp->buff_exp, env_node->value[k], tool);
		k++;
	}
	if (exp->buff_exp && env_node->value[k])
	{
		lst_add_back(&exp->token,
			new_lst(ft_strdup(exp->buff_exp, tool), tool));
		exp->buff_exp = NULL;
		exp->is_char = 0;
		while (env_node->value[k] && env_node->value[k] == ' ')
			k++;
		while (env_node->value[k])
		{
			k = hlp(env_node->value, k, exp, tool);
			while (env_node->value[k] && env_node->value[k] == ' ')
				k++;
		}
	}
}
