/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 08:14:34 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/15 09:57:05 by melkess          ###   ########.fr       */
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
