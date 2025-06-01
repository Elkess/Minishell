/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:52:06 by melkess           #+#    #+#             */
/*   Updated: 2025/05/31 12:30:23 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*filter_arg(char *s,int *sign, t_env *envh)
{
	char	**args;
	size_t	i;

	args = ft_split(s, 32); // maybe s need free
	if (!args || !args[0] || args && args[1])
		(print_err("exit: ", s, ": numeric Filter Range argument required\n"), free_envh(envh),  exit(255)); // free s and args
	i = 0;
	if (args[0] && (args[0][i] == '-' || args[0][i] == '+'))
		i++;
	if (i > 0 && args[0][i -1] == '-')
		*sign = -1;
	while (args[0] && args[0][i] && args[0][i] == '0')
		i++;
	return ((args[0] +i)); // the future me figure it out hihihihihihi
}

void	check_range(char *s, int sign, t_env *envh)
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
				(print_err("exit: ", s, ":48 numeric argument required"), free_envh(envh), exit(255)); // free s
			if (s[i] < exit_max[i])
				break ;
		}
		if (sign == -1)
		{
			if (s[i] > exit_min[i])
				(print_err("exit: ", s, ":55 numeric argument required\n"), free_envh(envh), exit(255)); // free s
		}
		i++;
	}
}

void	ft_exit(t_tree *cmd, int status, t_env *envh)
{
	size_t		i;
	char		*s;
	long long	n;
	int			sign;
//TODO: write exit when no args 
	if (!cmd)
		(free_envh(envh), exit (status));
	cmd->cmd++;
	sign = 1;
	i = 0;
	if (isatty(0) && isatty(1))
		ft_putstr_fd("exit\n", 2);
	if (!cmd || !cmd->cmd[0])
		(free_envh(envh), exit (0));
	while (cmd && cmd->cmd[i])
		i++;
	s = filter_arg(cmd->cmd[0], &sign, envh);
	if (sign == -1)
		s = ft_strjoin("-", s, 2); // free s
	if ((sign == -1 && ft_strlen(s) > 20) || sign == 1 && ft_strlen(s) > 19)
		(print_err("exit: ", s, ":83 numeric  argument required"),free_envh(envh), exit(255)); // free s
	if (i > 1)
		(ft_putstr_fd("minishell: exit: too many arguments\n", 2),free_envh(envh), exit (1)); // handle exit status, free s
	if ((sign == -1 && ft_strlen(s) == 20) || (sign == 1 && ft_strlen(s) == 19))
		check_range(s, sign, envh);
	(free_envh(envh), exit (ft_atol(s, sign) % 256)); // free s
}
