/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:29:09 by melkess           #+#    #+#             */
/*   Updated: 2025/05/17 15:25:56 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

long long	ft_atol(const char *str, int sign)
{
	size_t			i;
	long long		result;

	i = 0;
	result = 0;
	if (sign == -1)
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i] - 48;
		i++;
	}
	if (i != ft_strlen(str))
		(ft_putstr_fd(ft_strjoin("minishell: exit: ", ft_strjoin(str, ": Atol numeric  argument required\n")), 2)
			, exit(255));
	return (result * sign);
}
