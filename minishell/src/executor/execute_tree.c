/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 08:39:55 by melkess           #+#    #+#             */
/*   Updated: 2025/06/02 16:55:11 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
void static handle_ctrbackslash(int sig)
{
	write(1, "^\\Quit: 3\n", 11);
	exit(131);
}
int	execute_pipes(t_tree *tree, t_env *envh, t_tool	*tool)
{
	int		pipefd[2];
	pid_t	pids[2];
	int		status[2];
	int exit_stat;

	if (tree->type != NODE_PIPE)
		return (-1);
	if (pipe(pipefd) == -1)
		return (perror("Pipe failed"), -1);
	pids[0] = fork();
	if (pids[0] == -1)
		return (perror("Fork failed"), -1);
	if (pids[0] == 0)
	{
		(close(pipefd[0]), dup2(pipefd[1], 1));
		(close(pipefd[1]), exit(execute_tree(tree->left, envh, tool))); // SHoud it be exit and free_ evnh ???
	}
	pids[1] = fork();
	if (pids[1] == -1){perror("Fork failed");return (-1);}
	if (pids[1] == 0)
	{
		(close(pipefd[1]), dup2(pipefd[0], 0));
		(close(pipefd[0]), exit(execute_tree(tree->right, envh, tool))); // SHoud it be exit and free_ evnh
	}
	(close(pipefd[0]), close(pipefd[1]));
	signal(SIGINT, SIG_IGN);
	(waitpid(pids[0], &status[0], 0), waitpid(pids[1], &status[1], 0));
	g_signal = (WEXITSTATUS(status[1]) == 2 || WEXITSTATUS(status[0]) == 2) * 2 + (WEXITSTATUS(status[1]) == 3) * 3;
	if (WEXITSTATUS(status[1]) == 2 || WEXITSTATUS(status[1]) == 3)
		return (WEXITSTATUS(status[1]) + 128);
	return (WEXITSTATUS(status[1]));
}

int	execute_tree(t_tree *tree, t_env *envh, t_tool	*tool)
{
	int	status;

	status = 1;
	if (!tree)
		return (status);
	if (tree->type == NODE_COMMAND)
		return (executor(tree, envh, tool));
	if (tree->type == NODE_PARENTHS)
		return (executor(tree, envh, tool));
	if (tree->type == NODE_PIPE)
		return (execute_pipes(tree, envh, tool));
	if (tree->type == NODE_AND)
	{
		status = execute_tree(tree->left, envh, tool);
		if (status == 0)
			return (execute_tree(tree->right, envh, tool));
		return (status);
	}
	if (tree->type == NODE_OR)
	{
		status = execute_tree(tree->left, envh, tool);
		if (status != 0)
			return (execute_tree(tree->right, envh, tool));
		return (status);
	}
	return (status);
}
