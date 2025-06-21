/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 08:09:11 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/21 08:20:58 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	main_helper(t_tool *tool, char *line)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &tool->orig_termios);
	tool->inside_pipe = 0;
	free(line);
	clear_garbcoll(tool->grbg);
	colse_all();
}

void	ft_handle_signals(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = SIGINT;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	setup_signals(void)
{
	signal(SIGINT, ft_handle_signals);
	signal(SIGQUIT, SIG_IGN);
}

int	is_only_space(char *str)
{
	int	i;

	if (!str)
		return (1);
	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\r')
			return (0);
		i++;
	}
	free(str);
	return (1);
}

void	main_sigs(t_tool *tool)
{
	tool->signal = -2;
	tool->inside_pipe = 0;
	setup_signals();
}
