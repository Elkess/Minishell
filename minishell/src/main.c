/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:21:18 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/06 13:39:40 by sgmih            ###   ########.fr       */
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

t_token	*check_token(t_token **token, t_tool *tool)
{
	if (pars_err(token, tool))
		return (NULL);
	//update_lst_redir for fill s_redir 
	pars_trime(*token, tool);
	return (*token);
}

// void f()
// {
//     system("leaks -q minishell");
// }


int	main(int ac, char **av, char **env)
{

	char	*line;
	t_tree	*tree;
	t_tool	tool;
	
	(void)ac;
	(void)av;
	(void)env;

	while (1)
	{
		line = ft_get_prompt();
		//printf("Line input: %s\n", line);

		tree = parsing_input(line, &tool);

		//handle_input(line);
		free(line);
	}
	return (0);
}

