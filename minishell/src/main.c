/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:21:18 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/19 11:29:19 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	disable_echoctl(struct termios *orig_termios)
{
	struct termios term;

	tcgetattr(STDIN_FILENO, &term);
	*orig_termios = term;
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	restore_terminal(struct termios *orig_termios)
{
	tcsetattr(STDIN_FILENO, TCSANOW, orig_termios);
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

char	*ft_get_prompt(int exit_status, t_tool *tool)
{
	char	*prompt;

	disable_echoctl(&tool->orig_termios);
	if (exit_status == 0)
		prompt = readline("\033[0m\033[0;32m➜\033[0m  minishell$ ");
	else
		prompt = readline("\033[0;32m\033[0;31m➜\033[0m  minishell$ ");
	restore_terminal(&tool->orig_termios);
	if (prompt && prompt[0])
		add_history(prompt);
	if (!prompt)
	{
		ft_putstr_fd("exit\n", 2);
		exit(exit_status);
	}
	return (prompt);
}

void	fun_help(void)
{
	char	*line;

	line = getcwd(0, 0);
	if (!isatty(0) || !isatty(1))
	{
		ft_putstr_fd("minishell error: you are not in a tty \n", 2);
		if (line)
			(free(line), line = NULL);
		exit(1);
	}
	if (line)
		(free(line), line = NULL);
}

void	init_struct_tool_exec(t_tool *tool)
{
	tool->herdoc_err = 0;
	tool->err = 0;
	tool->signal = -2;
	tool->inside_pipe = 0;
	tcgetattr(STDIN_FILENO, &tool->orig_termios);
}

void	colse_all(void)
{
	int	i;

	i = 3;
	while (i < OPEN_MAX)
		close(i++);
}

void	main_helper(t_tool *tool, char *line)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &tool->orig_termios);
	if (tool->signal == SIGINT)
		ft_putstr_fd("\n", 2);
	else if (tool->signal == SIGQUIT)
		ft_putstr_fd("Quit: 3\n", 2);
	tool->inside_pipe = 0;
	free(line);
	clear_garbcoll(tool->grbg);
	colse_all();
}

int is_only_space(char *str)
{
    int i;
    
    if (!str)
        return (1);
    i = 0;
    while (str[i])
    {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\r')
            return (0);
        i++;
    }
    return (1);
}

int	main(int ac, char **av, char **env)
{//TODO: leaks, fd 
	char	*line;
	t_tree	*tree;
	t_tool	tool;
	t_env	*envh;

	init_struct_tool_exec(&tool);
	fun_help();
	envh = fill_env(env);
	while (av || ac)
	{
		tool.signal = -2;
		(tool.inside_pipe = 0, setup_signals());
		line = ft_get_prompt(tool.err, &tool);
		if (g_signal)
			(tool.err = 1, g_signal = 0);
		if (is_only_space(line))
		{
			free(line);
			continue;
		}
		tree = parsing_input(line, &tool);
		handle_herdocs(tree, envh, &tool);
		if (tool.herdoc_err == 1)
		{
			(tool.herdoc_err = 0, tool.err = 1);
			free(line);
			continue ;
		}
		else if (tree && line && *line)
			tool.err = execute_tree(tree, &envh, &tool);
		main_helper(&tool, line);
	}
}
