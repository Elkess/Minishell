/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 15:09:51 by melkess           #+#    #+#             */
/*   Updated: 2025/05/30 12:09:29 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_strnstr(const char *hs, const char *n, size_t len)
{
	size_t	i;
	size_t	j;

	if (n[0] == '\0')
		return ((char *)hs);
	i = 0;
	while (i < len && hs[i])
	{
		j = 0;
		while (i + j < len && hs[i + j] == n[j])
		{
			j++;
			if (n[j] == '\0')
				return ((char *)(hs + i));
		}
		i++;
	}
	return (NULL);
}
