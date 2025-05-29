/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:21:18 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/29 12:42:40 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_signal = 0;

void	ft_handle_signals(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = 1;
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	setup_signals(void)
{
	signal(SIGINT, ft_handle_signals);
	signal(SIGQUIT, SIG_IGN);
	rl_catch_signals = 0;
}

char	*ft_get_prompt(int exit_status)
{
	char	*prompt;

	prompt = readline("minishell$> ");
	if (prompt && prompt[0])
		add_history(prompt);
	if (!prompt)
	{
		printf("exit\n");
		exit(exit_status);
	}
	return (prompt);
}

void	fun_help(void)
{
	char	*line;

	line = getcwd(0, 0);
	if (!isatty(0) || !line)
	{
		ft_putstr_fd("ERROR\n", 2);
		if (line)
			(free(line), line = NULL);
		exit(1);
	}
	if (line)
		(free(line), line = NULL);
}

int	main(int ac, char **av, char **env)
{   // TODO: Export with args cd in erros
	char	*line;
	t_tree	*tree;
	t_tool	tool;
	t_env	*envh;

	line = NULL;
	fun_help();
	envh = fill_env(env);
	(void)ac;
	(void)av;
	setup_signals();
	while (1)
	{
		line = ft_get_prompt(0); // TO DO: give new prompt if line full of spaces 
		tree = parsing_input(line, &tool);
		if (tree && line && *line)
			tool.err = execute_tree(tree, envh, &tool);
		free(line);
		clear_garbcoll(tool.grbg); // TO DO: if we use clear_garbcoll(tool.grbg); we got segfault
		g_signal = 0;
	}
}
