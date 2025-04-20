/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:31:10 by sgmih             #+#    #+#             */
/*   Updated: 2025/04/19 11:56:30 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"

static char	*ft_substr(char *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;

	i = 0;
	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		len = 0;
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	str = (char *)malloc(len + 1);
	if (!str)
		return (NULL);
	while (i < len)
	{
		str[i] = s[start];
		i++;
		start++;
	}
	str[i] = '\0';
	return (str);
}

static	size_t	ft_count(char *s, char c)
{
	size_t	i;
	size_t	count;
	int		is_word;

	i = 0;
	count = 0;
	is_word = 0;
	while (s[i])
	{
		if (s[i] != c && !is_word)
		{
			count++;
			is_word = 1;
		}
		else if (s[i] == c)
			is_word = 0;
		i++;
	}
	return (count);
}

static	void	**ft_free(char **arr, size_t count)
{
	size_t	j;

	j = 0;
	while (j < count)
	{
		free (arr[j]);
		j++;
	}
	free(arr);
	return (NULL);
}

static char	**ft_fill_arr(char **arr, char c, char	*str)
{
	size_t		i;
	size_t		j;
	size_t		start;
	size_t		count;
	size_t		end;

	i = 0;
	j = 0;
	count = ft_count(str, c);
	while (j < count)
	{
		while (str[i] && str[i] == c)
			i++;
		if (str[i] && str[i] != c)
			start = i;
		while (str[i] && str[i] != c)
			i++;
		end = i - 1;
		arr[j] = ft_substr(str, start, (end - start) + 1);
		if (!arr[j])
			return (ft_free(arr, j), NULL);
		j++;
	}
	arr[j] = 0;
	return (arr);
}

char	**ft_split(char *s, char c)
{
	char			**arr;
	size_t			count;

	if (s == NULL)
		return (NULL);
	count = ft_count(s, c);
	arr = (char **)malloc((count + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	arr = ft_fill_arr(arr, c, s);
	return (arr);
}