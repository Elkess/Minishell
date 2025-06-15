/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 10:18:42 by melkess           #+#    #+#             */
/*   Updated: 2025/06/15 10:47:50 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_substr_env(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	i;
	size_t	lenfromstart;

	if (!s)
		return (NULL);
	if (start > ft_strlen(s))
		return (ft_strdup_env("", 0));
	lenfromstart = ft_strlen(s + start);
	if (len > lenfromstart)
		len = lenfromstart;
	substr = malloc(len +1);
	if (!substr)
		return (NULL);
	i = 0;
	while (i < len)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}
