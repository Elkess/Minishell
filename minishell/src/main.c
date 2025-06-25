/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:21:18 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/25 11:46:46 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	main_g_sig(t_tool *tool)
{
	tool->err = 1;
	g_signal = 0;
}

char	*ft_get_prompt(t_tool *tool)
{
	char	*prompt;

	disable_echoctl(&tool->orig_termios);
	prompt = readline("minishell$ ");
	restore_terminal(&tool->orig_termios);
	if (prompt && prompt[0])
		add_history(prompt);
	if (g_signal)
		main_g_sig(tool);
	if (!prompt)
	{
		ft_putstr_fd("exit\n", 2);
		clear_garbcoll(tool);
		exit(tool->err);
	}
	return (prompt);
}

void	setup_herdocs(t_tree *tree, t_tool *tool)
{
	if (count_heredocs(tree) > 16)
		(ft_putstr_fd(
				"minishell: maximum here-document count exceeded", 2),
			clear_garbcoll(tool), exit(2));
	handle_herdocs(tree, tool);
}

void	main_err(t_tool *tool, char *line)
{
	tool->herdoc_err = 0;
	tool->err = 1;
	free(line);
	clear_garbcoll(tool->grbg);
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_tree	*tree;
	t_tool	tool;
	t_env	*envh;

	init_struct_tool_exec(&tool, &envh, env);
	while (av || ac)
	{
		main_sigs(&tool);
		line = ft_get_prompt(&tool);
		if (is_only_space(line))
			continue ;
		tree = parsing_input(line, &tool);
		setup_herdocs(tree, &tool);
		if (tool.herdoc_err == 1)
		{
			main_err(&tool, line);
			continue ;
		}
		else if (tree && line && *line)
			tool.err = execute_tree(tree, &envh, &tool);
		main_helper(&tool, line);
	}
}
