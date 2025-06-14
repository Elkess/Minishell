/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:29:09 by melkess           #+#    #+#             */
/*   Updated: 2025/06/14 16:47:20 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

long long	ft_atol_ex(char *str, int sign, int count)
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
	if (count > 1)
		return (ft_putstr_fd("minishell: exit: too many arguments\n", 2), 1); // handle exit status, free s
	return (result * sign);
}
