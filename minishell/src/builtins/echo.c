/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:52:33 by melkess           #+#    #+#             */
/*   Updated: 2025/04/20 09:13:57 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor.h"

int	str_full_of_n(char *s)
{
	size_t	i;

	i = 2;
	if (!strcmp(s, "-n"))
		return (1);
	while (s[i] == 'n')
		i++;
	if (i == strlen(s))
		return (1);
	return (0);
}

void	echo(t_tree *tree)
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
		if (!strncmp(tree->cmd[i], "-n", 2))
		{
			if (str_full_of_n(tree->cmd[i]))
			{
				n = 1;
				i++;
			}
			else if (!str_full_of_n(tree->cmd[i]))
				return (print_rest(tree->cmd + i, tree->fd[0], n));
		}
		else
			return (print_rest(tree->cmd + i, tree->fd[1], n));
	}
}