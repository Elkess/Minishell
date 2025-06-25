/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 12:19:04 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/25 12:23:47 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_var(char c)
{
	return (c == ' ' || c == '\t');
}

static int	nbr_words(const char *s)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	if (s[0] != '\0' && !is_var(s[0]))
		n++;
	while (s[i])
	{
		if (is_var(s[i]) && s[i + 1] != '\0' && !is_var(s[i + 1]))
			n++;
		i++;
	}
	return (n);
}

static char	*one_word(const char *s, int *i, t_tool *tool)
{
	int	start;
	int	end;

	while (s[*i] && is_var(s[*i]))
		(*i)++;
	start = *i;
	while (s[*i] && !is_var(s[*i]))
		(*i)++;
	end = *i;
	return (ft_substr(s, start, end - start, tool));
}

char	**ft_split_var(char const *s, t_tool *tool)
{
	char	**splitedstr;
	int		nbrwords;
	int		i;
	int		j;

	if (!s)
		return (NULL);
	nbrwords = nbr_words(s);
	splitedstr = (char **) malloc((nbrwords + 1) * sizeof(char *));
	if (!splitedstr)
		return (NULL);
	add_to_grbg(&tool->grbg, splitedstr);
	i = 0;
	j = 0;
	while (s[i] && j < nbrwords)
	{
		splitedstr[j] = one_word(s, &i, tool);
		if (!splitedstr[j])
			return (NULL);
		j++;
	}
	splitedstr[j] = NULL;
	return (splitedstr);
}
