/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:21:11 by melkess           #+#    #+#             */
/*   Updated: 2025/06/20 23:48:51 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	copy_strings(char *dest, char *s1, char *s2)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		dest[i + j] = s2[j];
		j++;
	}
	dest[i + j] = '\0';
}

static void	free_strings(char *s1, char *s2, int flag)
{
	if (flag == 1)
		free(s1);
	else if (flag == 2)
		free(s2);
	else if (flag == 3)
	{
		free(s1);
		free(s2);
	}
}

char	*ft_strjoin_env(char *s1, char *s2, int flag)
{
	char	*newstr;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup_env(s2, 0));
	if (!s2)
		return (ft_strdup_env(s1, 0));
	newstr = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!newstr)
		return (NULL);
	copy_strings(newstr, s1, s2);
	free_strings(s1, s2, flag);
	return (newstr);
}
