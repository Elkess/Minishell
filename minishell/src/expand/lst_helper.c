/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 08:14:34 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/22 08:37:16 by melkess          ###   ########.fr       */
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
