/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_sett.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 08:13:49 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/25 11:47:10 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	fun_help(void)
{
	char	*line;

	line = getcwd(0, 0);
	if (!isatty(0) || !isatty(1) || !line)
	{
		if (!line)
			ft_putstr_fd(
				"minishell error: the program cannot run in orphan path\n", 2);
		else
			ft_putstr_fd("minishell error: you are not in a tty \n", 2);
		if (line)
			(free(line), line = NULL);
		exit(1);
	}
	if (line)
		(free(line), line = NULL);
}

void	init_struct_tool_exec(t_tool *tool, t_env **envh, char **env)
{
	tool->herdoc_err = 0;
	tool->err = 0;
	tool->signal = -2;
	tool->inside_pipe = 0;
	tool->pwd_backup = NULL;
	tcgetattr(STDIN_FILENO, &tool->orig_termios);
	fun_help();
	*envh = fill_env(env);
}

void	close_all(void)
{
	int	i;

	i = 3;
	while (i < OPEN_MAX)
		close(i++);
}

void	disable_echoctl(struct termios *orig_termios)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	*orig_termios = term;
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	restore_terminal(struct termios *orig_termios)
{
	tcsetattr(STDIN_FILENO, TCSANOW, orig_termios);
}
