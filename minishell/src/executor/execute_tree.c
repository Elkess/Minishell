/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 08:39:55 by melkess           #+#    #+#             */
/*   Updated: 2025/06/23 15:22:52 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	can_we_fork(t_tree *tree)
{
	pid_t	pid;
	int		status;
	int		left_ok;
	int		right_ok;

	if (!tree)
		return (1);
	if (tree->type == NODE_PIPE)
	{
		pid = fork();
		if (pid < 0)
			return (perror("fork"), 0);
		if (pid == 0)
		{
			left_ok = can_we_fork(tree->left);
			right_ok = can_we_fork(tree->right);
			exit(!(left_ok && right_ok));
		}
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			return (0);
	}
	return (1);
}

int	failed_fork(t_tool *tool)
{
	perror("Fork failed");
	if (tool->inside_pipe == 1)
		exit (1);
	else
		return (1);
}

int	execute_pipes(t_tree *tree, t_env **envh, t_tool *tool)
{
	int		pipefd[2];
	pid_t	pids[2];

	if (tree->type != NODE_PIPE)
		return (-1);
	if (pipe(pipefd) == -1)
		return (perror("Pipe error"), 1);
	pids[0] = fork();
	if (pids[0] == -1)
		return (failed_fork(tool));
	tool->inside_pipe = 1;
	if (pids[0] == 0)
		(close(pipefd[0]), dup2(pipefd[1], 1),
			close(pipefd[1]), exit (execute_tree(tree->left, envh, tool)));
	1 && (tool->err = 0, pids[1] = fork());
	if (pids[1] < 0)
		return (failed_fork(tool));
	if (pids[1] == 0)
		(close(pipefd[1]), dup2(pipefd[0], 0), close(pipefd[0]),
			exit(execute_tree(tree->right, envh, tool)));
	(signal(SIGINT, SIG_IGN), close(pipefd[0]),
		close(pipefd[1]), waitpid(pids[1], &tool->status, 0));
	while (wait(NULL) != -1)
		;
	return (WEXITSTATUS(tool->status));
}

int	exec_tree_helper(t_tree *tree, t_env **envh, t_tool	*tool)
{
	if (can_we_fork(tree))
		return (execute_pipes(tree, envh, tool));
	return (1);
}

int	execute_tree(t_tree *tree, t_env **envh, t_tool	*tool)
{
	int	status;

	status = 1;
	if (!tree)
		return (status);
	else if (tree->type == NODE_COMMAND)
		return (executor(tree, envh, tool));
	else if (tree->type == NODE_PARENTHS)
		return (executor(tree, envh, tool));
	else if (tree->type == NODE_PIPE)
		return (exec_tree_helper(tree, envh, tool));
	else if (tree->type == NODE_AND)
	{
		status = execute_tree(tree->left, envh, tool);
		if (status == 0)
			return (execute_tree(tree->right, envh, tool));
	}
	else if (tree->type == NODE_OR)
	{
		status = execute_tree(tree->left, envh, tool);
		if (status != 0)
			return (execute_tree(tree->right, envh, tool));
	}
	return (status);
}
