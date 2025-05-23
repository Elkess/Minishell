/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:21:11 by melkess           #+#    #+#             */
/*   Updated: 2025/05/20 14:35:50 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_strjoin_env(char const *s1, char const *s2)
{
	char	*newstr;
	char	*head;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup_env(s2, 0));
	if (!s2)
		return (ft_strdup_env(s1, 0));
	newstr = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) +1);
	if (!newstr)
		return (NULL);
	head = newstr;
	while (*s1)
		*newstr++ = *s1++;
	while (*s2)
		*newstr++ = *s2++;
	*newstr = '\0';
	return (head);
}
