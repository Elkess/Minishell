/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_funcs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:39:01 by melkess           #+#    #+#             */
/*   Updated: 2025/06/19 15:59:45 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"



int	is_valid_key(char *key)
{
	size_t	i;

	if (!key || !*key)
		return (1);
	i = 0;
	if (!ft_isalpha(key[i]) && key[i] != '_')
		return (0);
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	print_rest(char **s, int n, t_tool *tool)
{
	size_t	i;
	char	*buffer;

	buffer = NULL;
	i = 0;
	while (s[i])
	{
		buffer = ft_strjoin(buffer, s[i++], tool);
		if (s[i])
			buffer= ft_strjoin(buffer, " ", tool);
	}
	if (!n)
		buffer= ft_strjoin(buffer, "\n", tool);
	write(1, buffer, ft_strlen(buffer));
}

void	free_twod(char **s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
		free(s[i++]);
	free(s);
}