/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:52:33 by melkess           #+#    #+#             */
/*   Updated: 2025/05/05 10:50:02 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor.h"

int	str_full_of_n(char *s)
{
	size_t	i;

	i = 2;
	if (!ft_strcmp(s, "-n"))
		return (1);
	while (s[i] == 'n')
		i++;
	if (i == ft_strlen(s))
		return (1);
	return (0);
}

int	echo(t_tree *tree)
{
	size_t	i;
	int		n;

	tree->cmd++;
	i = 0;
	n = 0;
	if (!tree->cmd[i])
		printf("\n");
	while (tree->cmd[i])
	{
		if (!ft_strncmp(tree->cmd[i], "-n", 2) && str_full_of_n(tree->cmd[i]))
		{
				n = 1;
				i++;
		} 
		else
		{
			print_rest(tree->cmd + i, n); // fail if write fails -> 1
			break ;
		}
	}
	return (0);
}
