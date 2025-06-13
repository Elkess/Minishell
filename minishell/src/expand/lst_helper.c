/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 08:14:34 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/11 08:30:22 by sgmih            ###   ########.fr       */
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

char	*ft_strdup_exc(const char *s1, t_tool *tool)
{
	size_t	len;
	size_t	i;
	char	*ptr;

	i = 0;
	if (!s1)
		return (NULL);
	len = ft_strlen(s1);
	ptr = malloc(sizeof(char) * (len + 1));
	if (!ptr)
		return (NULL);
    add_to_grbg(&tool->grbg, ptr);
	while (i < len)
	{
		ptr[i] = s1[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}