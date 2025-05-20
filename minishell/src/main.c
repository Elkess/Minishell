/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:21:18 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/20 10:15:50 by sgmih            ###   ########.fr       */
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
void	handle_herdocs(t_tree *tree)
{
	if (!tree)
		return ;
	here_docs(tree->redirs);
	handle_herdocs(tree->left);
	handle_herdocs(tree->right);
}

int	main(int ac, char **av, char **env)
{
	//atexit(f);
	char	*line;
	t_tree	*tree;
	t_tool	tool;
	t_env	*envh;

	envh = fill_env(env);
	
	(void)ac;
	(void)av;
	setup_signals();
	while (1)
	{
		line = ft_get_prompt(0);
		// TODO: give new prompt if line full of spaces 
		tree = parsing_input(line, &tool);
		handle_herdocs(tree);
		if (line && *line)
			printf("exit status: %d\n", execute_tree(tree, envh));
		free(line);
		g_signal = 0;
	}
	return (0);
}

