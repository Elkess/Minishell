/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_funcs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:39:01 by melkess           #+#    #+#             */
/*   Updated: 2025/05/21 19:04:33 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"



int	is_valid_key(char *key)
{
	size_t	i;

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

void	print_rest(char **s, int n)
{
	size_t	i;
	char	*buffer;

	buffer = NULL;
	i = 0;
	while (s[i])
	{
		buffer = ft_strjoin(buffer, s[i++], 1); // leaks
		if (s[i])
			buffer= ft_strjoin(buffer, " ", 1); //TODO: handle tabs // leaks
	}
	if (!n)
		buffer= ft_strjoin(buffer, "\n", 1); // leaks
	write(1, buffer, ft_strlen(buffer));
	free(buffer);
}

void	free_twod(char **s)
{
	size_t	i;

	i = 0;
	while (s[i])
		free(s[i++]);
	free(s);
}