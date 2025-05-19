/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:21:18 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/18 14:24:05 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_get_prompt(void)
{
	char	*prompt;

	prompt = readline("minishell$> ");
	if (prompt && prompt[0])
		add_history(prompt);
	if (!prompt)
	{
		printf("exit\n");
		exit(0);
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

	if (!isatty(0))
	{
		ft_putstr_fd("Error\n", 2);
		exit(1);
	}
	envh = fill_env(env);
	// search_for_defaults(envh, "PWD");
	(void)ac;
	(void)av;
	while (1)
	{
		line = ft_get_prompt();
		// TODO: give new prompt if line full of spaces 
		tree = parsing_input(line, &tool);
		handle_herdocs(tree);
		if (line && *line)
			printf("exit status: %d\n", execute_tree(tree, envh));
		free(line);
	}
	return (0);
}

