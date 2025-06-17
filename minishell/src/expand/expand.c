/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 08:56:07 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/17 08:06:40 by sgmih            ###   ########.fr       */
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

static int	handle_special(t_expand *expand, t_tool *tool, char *str, int status)
{
	if (str[expand->j] == '?')
	{
		char *var_name = ft_itoa(status, tool);
		if (var_name)
			lst_add_back(&expand->token, new_lst(ft_strdup(var_name, tool), tool));
		expand->j++;
		return (1);
	}
	else if (str[expand->j] == '$')
	{
		expand->buff_exp = strjoin_char(expand->buff_exp, '$', tool);
		expand->is_char = 1;
		expand->j++;
		return (1);
	}
	else if (str[expand->j] == '0')
	{
		lst_add_back(&expand->token, new_lst(ft_strdup("minishell", tool), tool));
		expand->j++;
		return (1);
	}
	else if (!ft_isalnum(str[expand->j]) && str[expand->j] != '_')
	{
		expand->buff_exp = strjoin_char(expand->buff_exp, '$', tool);
		expand->is_char = 1;
		return (1);
	}
	return (0);
}

static char	*extract_var_name(t_expand *expand, t_tool *tool, char *str)
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
		lst_add_back(&expand->token, new_lst(ft_strdup(expand->buff_exp, tool), tool));
		expand->buff_exp = NULL;
	}
	i = 0;
	while (words[i])
	{
		lst_add_back(&expand->token, new_lst(ft_strdup(words[i], tool), tool));
		i++;
	}
}

static void	handle_space_expansion(t_expand *expand, t_tool *tool, char **words)
{
	int	i;

	if (has_space(words[0]) == 2)
		handle_multiple_spaces(expand, tool, words);

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
			lst_add_back(&expand->token, new_lst(ft_strdup(expand->buff_exp, tool), tool));
		if (words[1])
			expand->buff_exp = ft_strjoin(NULL, words[1], tool);
		else
			expand->buff_exp = NULL;
	}
}

static void	expand_env_variable(t_expand *expand, t_tool *tool, t_env *env_node)
{
	char	**words;

	if (env_node->value && ft_strchr(env_node->value, '*'))
		expand->is_wildcard = 1;

	if (expand->flg != '"' && !expand->is_there_export && has_space(env_node->value))
	{
		words = ft_split(env_node->value, ' ', tool);
		handle_space_expansion(expand, tool, words);
	}
	else
	{
		expand->buff_exp = ft_strjoin(expand->buff_exp, env_node->value, tool);
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

static void	init_expand(t_expand *expand, t_tree *tree)
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

static void	process_wildcard_expansion(t_expand *expand, t_tool *tool)
{
	char	**list_wld;
	int		k;

	list_wld = expand_wildcard(expand->buff_exp, tool);
	k = 0;
	while (list_wld && list_wld[k])
	{
		lst_add_back(&expand->token, new_lst(ft_strdup(list_wld[k], tool), tool));
		k++;
	}
}

static void	cmd_arg(t_expand *expand, char *cmd_arg, int exit_status, t_tool *tool)
{
	expand->j = 0;
	expand->is_char = 0;
	expand->is_wildcard = 0;
	
	if (!cmd_arg)
		return;
		
	while (cmd_arg[expand->j])
	{
		if (cmd_arg[expand->j] == '"' || cmd_arg[expand->j] == '\'')
		{
			expand_quote(expand, cmd_arg, exit_status, tool);
			expand->j++;
		}
		else if (cmd_arg[expand->j] == '$' && expand->flg != '\'')
			expand_dollar(expand, tool, cmd_arg, exit_status);
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
