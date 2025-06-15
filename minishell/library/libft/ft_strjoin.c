/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:21:11 by melkess           #+#    #+#             */
/*   Updated: 2025/06/15 11:06:59 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_strjoin(char *s1, char *s2, t_tool *tool)
{
	char	*newstr;
	char	*head;
	size_t	i;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2, tool));
	if (!s2)
		return (ft_strdup(s1, tool));
	newstr = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) +1);
	if (!newstr)
		return (NULL);
	add_to_grbg(&tool->grbg, newstr);
	head = newstr;
	i = 0;
	while (s1[i])
		*newstr++ = s1[i++];
	i = 0;
	while (s2[i])
		*newstr++ = s2[i++];
	*newstr = '\0';
	return (head);
}
