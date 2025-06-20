/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:55:36 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/20 18:21:28 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_file_exp(t_redir *red, char **expand_file, t_tool *tool)
{
	int	file_count;

	if (!expand_file)
	{
		red->is_ambiguous = 1;
		return ;
	}
	file_count = 0;
	while (expand_file[file_count])
		file_count++;
	if (file_count == 1)
	{
		red->file = ft_strdup(expand_file[0], tool);
		red->is_ambiguous = 0;
	}
	else
		red->is_ambiguous = 1;
}

void	expand_redir(t_tree *tree, t_tool *tool, int status)
{
	t_redir	*red;
	t_tree	temp_tree;
	char	**expand_file;
	char	*temp_cmd[2];

	red = tree->redirs;
	while (red)
	{
		if (red->type == 0 || red->type == 1 || red->type == 2)
		{
			temp_cmd[0] = red->file;
			temp_cmd[1] = NULL;
			temp_tree.cmd = temp_cmd;
			expand_file = handel_expand(&temp_tree, status, tool);
			handle_file_exp(red, expand_file, tool);
		}
		red = red->next;
	}
}

static char	*handle_var_case(char *line, int *i, char *result, t_tool *tool)
{
	int		j;
	char	*var_name;
	t_env	*expand_line;

	j = *i;
	while (line[*i] && valid_char(line[*i]))
		(*i)++;
	var_name = ft_substr(line, j, *i - j, tool);
	if (var_name)
	{
		expand_line = sh_env(tool->envh, var_name);
		if (expand_line && expand_line->value)
			result = ft_strjoin(result, expand_line->value, tool);
	}
	return (result);
}

static char	*handle_dollar_exp(char *line, int *i, char *result, t_tool *tool)
{
	char	*var_name;

	var_name = NULL;
	(*i)++;
	if (line[*i] == '?')
	{
		result = ft_strjoin(result, ft_itoa(tool->status_exp, tool), tool);
		(*i)++;
	}
	else if (line[*i] == '$')
	{
		result = strjoin_char(result, '$', tool);
		(*i)++;
	}
	else if (line[*i] == '0')
	{
		result = ft_strjoin(result, "minishell", tool);
		(*i)++;
	}
	else if (valid_char(line[*i]))
		result = handle_var_case(line, i, result, tool);
	else
		result = strjoin_char(result, '$', tool);
	return (result);
}

char	*expand_herdoc_content(char *line, t_tool *tool, int status)
{
	t_env	*expand_line;
	char	*result;
	int		i;

	i = 0;
	expand_line = NULL;
	result = NULL;
	tool->status_exp = status;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1])
			result = handle_dollar_exp(line, &i, result, tool);
		else
		{
			result = strjoin_char(result, line[i], tool);
			i++;
		}
	}
	return (result);
}
