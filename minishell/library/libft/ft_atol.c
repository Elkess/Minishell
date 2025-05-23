/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:29:09 by melkess           #+#    #+#             */
/*   Updated: 2025/05/21 14:17:59 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

long long	ft_atol(char *str, int sign)
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
		(print_err("exit: ", str, "Atol numeric  argument required"), exit(255));
	return (result * sign);
}
