/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 08:14:34 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/25 14:57:22 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*new_lst(void *content, t_tool *tool)
{
	t_token	*new_node;

	if (!content)
		return (NULL);
	new_node = (t_token *) malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	add_to_grbg(&tool->grbg, new_node);
	new_node->value = content;
	new_node->next = NULL;
	return (new_node);
}

int	one_wildcard(char *str)
{
	while (*str)
	{
		if (*str != '*')
			return (0);
		str++;
	}
	return (1);
}

int	is_only_whitespace(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (1);
	while (str[i])
	{
		if (!is_space(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	check_export_split(t_expand *expand)
{
	expand->env_split = 0;
	if ((!expand->is_there_export || !expand->flg_split))
	{
		if (expand->val_quotes)
		{
			expand->env_split = 0;
			return ;
		}
		expand->env_split = 1;
	}
	else
		expand->env_split = 0;
}

void	check_if_quoted(char *val, t_expand *expand)
{
	size_t	len;

	if (val)
	{
		len = strlen(val);
		if (len >= 2)
		{
			if ((val[0] == '"' && val[len - 1] == '"')
				|| (val[0] == '\'' && val[len - 1] == '\''))
			{
				expand->val_quotes = 1;
			}
		}
	}
}
