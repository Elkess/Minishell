/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:21:18 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/13 15:27:34 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void disable_echoctl(struct termios *orig_termios)
{
	struct termios term;

	tcgetattr(STDIN_FILENO, &term);
	*orig_termios = term; // 11011  &  11011
 	term.c_lflag = term.c_lflag & ~ECHOCTL; // booooring 536872399 536872335
	// term.c_lflag -= 64;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void restore_terminal(struct termios *orig_termios)
{
	tcsetattr(STDIN_FILENO, TCSANOW, orig_termios);
}

void	ft_handle_signals(int sig)
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	setup_signals(void)
{
	signal(SIGINT, ft_handle_signals);
	signal(SIGQUIT, SIG_IGN);
}

char	*ft_get_prompt(int exit_status)
{
	char	*prompt;
	struct termios orig_termios;

	disable_echoctl(&orig_termios);
	prompt = readline("minishell$> ");
	restore_terminal(&orig_termios);
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
	if (!isatty(0) || !isatty(1)|| !line)
	{
		ft_putstr_fd("ERROR\n", 2);
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
	tool->signal = -1;
	tool->fork = 0;
}

void	main_helper(t_tool *tool, char *line)
{
	if (tool->signal == SIGINT)
		ft_putstr_fd("\n", 1);
	else if (tool->signal == SIGQUIT)
		ft_putstr_fd("Quit: 3\n", 1);
	free(line);
	clear_garbcoll(tool->grbg);
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
		tool.signal = -1;
		(tool.fork = 0, setup_signals());
		line = ft_get_prompt(tool.err);
		tree = parsing_input(line, &tool);
		handle_herdocs(tree, envh, &tool);
		if (tool.herdoc_err == 1)
		{
			(tool.herdoc_err = 0, tool.err = 1);
			continue ;
		}
		else if (tree && line && *line)
			tool.err = execute_tree(tree, envh, &tool);
		main_helper(&tool, line);
	}
}
