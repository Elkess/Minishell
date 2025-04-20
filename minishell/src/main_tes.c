/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_tes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:21:18 by sgmih             #+#    #+#             */
/*   Updated: 2025/04/20 09:13:13 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <readline/readline.h>
#include "../include/executor.h"
//cc -lreadline src/main.c src/builtins/*.c src/executor/*.c src/executor/env/*.c  library/libft/*.c -o minishell && env -i ./minishell
static char	*ft_get_prompt(int exit_status)
{
	char	*prompt;

	prompt = readline("minishell$>$ ");
	if (prompt && prompt[0])
		add_history(prompt);
	if (!prompt)
	{
		printf("exit\n");
		exit(exit_status);
	}
	return (prompt);
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	char	**cmd;
    size_t  i = 0;
    t_tree  *tree;
	t_env	*envh;

	envh = fill_env(env);
    tree = malloc(sizeof(t_tree));
    // tree->redirs = malloc(sizeof(t_redir));
	while (1)
	{
		line = ft_get_prompt(0);
		// tree->type = NODE_COMMAND;
		tree->cmd = ft_split(line, ' ');
		// if (!ft_strcmp(tree->cmd[0], ">"))
		// {
		// 	tree->redirs->type = REDIR_OUT;
		// 	tree->redirs->file = tree->cmd[1];
		// 	tree->redirs->next = NULL;
		// 	tree->cmd[0] = NULL;
		// }
		// else if (!ft_strcmp(tree->cmd[0], "<"))
		// {
		// 	tree->redirs->type = REDIR_IN;
		// 	tree->redirs->file = tree->cmd[1];
		// 	tree->cmd[0] = NULL;
		// }
		// else
		// 	tree->redirs = NULL;
		// tree->cmd = ft_split(ft_strdup(" cat "), ' ');
		if (line && *line)
			executor(tree, envh);
		free(line);
	}
	return (0);
}