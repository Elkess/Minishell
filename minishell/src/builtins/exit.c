/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:52:06 by melkess           #+#    #+#             */
/*   Updated: 2025/05/18 13:14:03 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*filter_arg(char *s,int *sign)
{
	char	**args;
	size_t	i;

	args = ft_split(s, 32);
	if (!args || !args[0] || args && args[1])
		(ft_putstr_fd(ft_strjoin("exit\nminishell: exit: ", ft_strjoin(s, ": numeric Filter Range argument required\n")), 2), exit(255));
	i = 0;
	if (args[0] && (args[0][i] == '-' || args[0][i] == '+'))
		i++;
	if (i > 0 && args[0][i -1] == '-')
		*sign = -1;
	while (args[0] && args[0][i] && args[0][i] == '0')
		i++;
	return ((args[0] +i));
}

void	check_range(char *s, int sign)
{
	char	*exit_max;
	char	*exit_min;
	size_t	i;

	i = 0;
	exit_max = "9223372036854775807";
	exit_min = "-9223372036854775808";

	while (s[i])
	{
		if (sign == 1)
		{
			if (s[i] > exit_max[i])
				(ft_putstr_fd(ft_strjoin("exit\nminishell: exit: ", ft_strjoin(s, ": numeric Check Range1 argument required\n")), 2), exit(255));
			if (s[i] < exit_max[i])
				break ;
		}
		if (sign == -1)
		{
			if (s[i] > exit_min[i])
				(ft_putstr_fd(ft_strjoin("exit\nminishell: exit: ", ft_strjoin(s, ": numeric Check Range2 argument required\n")), 2), exit(255));
		}
		i++;
	}
}

void	ft_exit(t_tree *cmd, int status)
{
	size_t		i;
	char		*s;
	long long	n;
	int			sign;
//TODO: write exit when no args 
	if (!cmd)
		exit (status);
	cmd->cmd++;
	sign = 1;
	i = 0;
	if (!cmd || !cmd->cmd[0])
		exit (0);
	while (cmd && cmd->cmd[i])
		i++;
	s = filter_arg(cmd->cmd[0], &sign);
	if (sign == -1)
		s = ft_strjoin("-", s);
	if ((sign == -1 && ft_strlen(s) > 20) || sign == 1 && ft_strlen(s) > 19)
		(ft_putstr_fd(ft_strjoin("exit\nminishell: exit: ", ft_strjoin(s, ": numeric Exit argument required\n")), 2), exit(255));
	if (i > 1)
		(ft_putstr_fd("exit\nminishell: exit: too many arguments\n", 2), exit (10));
	if ((sign == -1 && ft_strlen(s) == 20) || (sign == 1 && ft_strlen(s) == 19))
		check_range(s, sign);
	exit (ft_atol(s, sign) % 256);
}
