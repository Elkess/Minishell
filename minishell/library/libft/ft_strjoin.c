/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:21:11 by melkess           #+#    #+#             */
/*   Updated: 2025/05/21 20:36:43 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_strjoin(char *s1, char *s2, int flag)
{
	char	*newstr;
	char	*head;
	size_t	i;

	i = 0;
	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	newstr = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) +1);
	if (!newstr)
		return (NULL);
	head = newstr;
	while (s1[i])
		*newstr++ = s1[i++];
	i = 0;
	while (s2[i])
		*newstr++ = s2[i++];
	*newstr = '\0';
	if (flag == 1)
		free(s1);
	if (flag == 2)
		free(s2);
	if (flag == 3)
		(free(s1), free(s2));
	return (head);
}
