/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_trime.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 07:38:27 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/03 07:46:48 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	count;
	size_t	lens;

	count = 0;
	lens = 0;
	while (src[lens])
		lens++;
	if (dstsize == 0)
		return (lens);
	while (src[count] != '\0' && count + 1 < dstsize)
	{
		dst[count] = src[count];
		count++;
	}
	dst[count] = '\0';
	return (lens);
}

static char	*check(t_tool *tool)
{
	char	*newstr;

	newstr = (char *)malloc(1 * sizeof(char));
	if (!newstr)
		return (NULL);
	add_to_grbg(&tool->grbg, newstr);
	newstr[0] = '\0';
	return (newstr);
}

static char	*ft_strtrim(const char *s1, const char *set, t_tool *tool)
{
	size_t	start;
	size_t	end;
	size_t	len_new_dst;
	char	*result;

	if (!s1)
		return (NULL);
	if (ft_strlen(s1) == 0)
		return (check(tool));
	if (ft_strlen(set) == 0 || !set)
		return (ft_strdup(s1, tool));
	end = ft_strlen(s1) - 1;
	start = 0;
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	while (end > start && ft_strchr(set, s1[end]))
		end--;
	len_new_dst = end - start + 1;
	result = (char *)malloc((len_new_dst + 1) * sizeof(char));
	if (!result)
		return (NULL);
	add_to_grbg(&tool->grbg, result);
	ft_strlcpy(result, &s1[start], len_new_dst + 1);
	return (result);
}

void	pars_trime(t_token *head, t_tool *tool)
{
	char	*arr;

	if (!head)
		return ;
	while (head)
	{
		arr = head->value;
		head->value = ft_strtrim(arr, " ", tool);
		head = head->next;
	}
}