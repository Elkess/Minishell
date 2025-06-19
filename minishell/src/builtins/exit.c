/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:52:06 by melkess           #+#    #+#             */
/*   Updated: 2025/06/19 10:44:06 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
// TODO : add clear grb 
char	*filter_arg(char *s,int *sign, t_env *envh, t_tool *tool)
{
	char	**args;
	size_t	i;

	args = ft_split(s, 32, tool); // maybe s need free
	if (!args || !args[0] || args && args[1])
		(print_err("exit: ", s, ": numeric Filter Range argument required\n"), free_envh(envh), exit(255)); // free s and args
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
				(print_err("exit: ", s, ":48 numeric argument required"),free_envh(envh), exit(255)); // free s
			if (s[i] < exit_max[i])
				break ;
		}
		if (sign == -1)
		{
			if (s[i] > exit_min[i])
				(print_err("exit: ", s, ":55 numeric argument required\n"),free_envh(envh), exit(255)); // free s
		}
		i++;
	}
}

int	ft_exit(t_tree *cmd, int status, t_env *envh, t_tool *tool)
{
	size_t		i;
	char		*s;
	long long	n;
	int			sign;
//TODO: write exit when no args 
	if (!cmd)
		(free_envh(envh), exit (status));
	cmd->cmd++;
	if (!*cmd->cmd)
		(free_envh(envh), exit(tool->err));
	sign = 1;
	i = 0;
	if (isatty(0) && isatty(1))
		ft_putstr_fd("exit\n", 2);
	if (!cmd || !cmd->cmd[0])
		(free_envh(envh), exit (0));
	while (cmd && cmd->cmd[i])
		i++;
	s = filter_arg(cmd->cmd[0], &sign, envh, tool);
	if (sign == -1)
		s = ft_strjoin("-", s, tool); // free s
	if ((sign == -1 && ft_strlen(s) > 20) || sign == 1 && ft_strlen(s) > 19)
		(print_err("exit: ", s, ":83 numeric  argument required"), free_envh(envh),exit(255)); // free s
	if ((sign == -1 && ft_strlen(s) == 20) || (sign == 1 && ft_strlen(s) == 19))
		check_range(s, sign, envh);
	n = ft_atol_ex(s, sign, i) % 256; // free s
	if (i == 1)
		(free_envh(envh), exit (n));
	else
		return (n);
	return (0);
}
