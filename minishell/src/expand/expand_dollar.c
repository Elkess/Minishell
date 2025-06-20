/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:40:55 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/20 14:58:17 by melkess          ###   ########.fr       */
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
		lst_add_back(&exp->token, new_lst(ft_strdup("minishell", tool), tool));
		exp->j++;
		return (1);
	}
	else if (!ft_isalnum(str[exp->j]) && str[exp->j] != '_')
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

static void	handle_multiple_spaces(t_expand *expand, t_tool *tool, char **words)
{
	int	i;

	if (expand->buff_exp)
	{
		lst_add_back(&expand->token,
			new_lst(ft_strdup(expand->buff_exp, tool), tool));
		expand->buff_exp = NULL;
	}
	i = 0;
	while (words[i])
	{
		lst_add_back(&expand->token, new_lst(ft_strdup(words[i], tool), tool));
		i++;
	}
	return ;
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

	if (!env_node->value || env_node->value[0] == '\0')
		return ;
	if (env_node->value && ft_strchr(env_node->value, '*'))
		exp->is_wildcard = 1;
	if (exp->flg != '"' && !exp->is_there_export
		&& has_space(env_node->value))
	{
		words = ft_split(env_node->value, ' ', tool);
		if (!words || !words[0])
			return ;
		if (has_space(words[0]) == 2)
			handle_multiple_spaces(exp, tool, words);
		handle_space_expansion(exp, tool, words);
	}
	else
	{
		exp->buff_exp = ft_strjoin(exp->buff_exp, env_node->value, tool);
	}
}
