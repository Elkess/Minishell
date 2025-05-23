/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 10:14:10 by melkess           #+#    #+#             */
/*   Updated: 2025/05/21 14:18:39 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_free(void *ptr, int flag)
{
	static void	*lst[INT_MAX];
	static int	i;
	int			j;

	if (flag && *lst)
	{
		j = 0;
		while (lst[j])
			free(lst[j++]);
		*lst = NULL;
	}
	else
		lst[i++] = ptr;
}

void	ft_exits(int n)
{
	ft_free(NULL, 1);
	exit(n);
}

void	*ft_malloc(size_t n)
{
	void	*ptr;

	ptr = malloc(n);
	if (!ptr)
		ft_exits(1);
	ft_free(ptr, 0);
	return (ptr);
}
