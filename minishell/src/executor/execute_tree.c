/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 08:39:55 by melkess           #+#    #+#             */
/*   Updated: 2025/06/19 15:57:41 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_pid	*add_to_pids_list(t_pid *head, int val)
{
	t_pid	*node;
	t_pid	*backup;

	node = malloc(sizeof(t_pid));
	if (!node)
		(perror(""), exit (1));
	node->value = val;
	node->next = NULL;
	if (!head)
		return (node);
	else
	{
		backup = head;
		while (head->next)
			head = head->next;
		head->next = node;
	}
	return (backup);
}
int can_we_fork(t_tree *tree)
{
	pid_t pid;

	if (!tree)
		return (1);
	if (tree->type == NODE_PIPE)
	{
		pid = fork();
		if (pid < 0)
		{
			perror("Fork failed during can_we_fork check");
			return (0);
		}
		if (pid == 0)
		{
			int left_ok = can_we_fork(tree->left);
			int right_ok = can_we_fork(tree->right);
			exit(!(left_ok && right_ok));
		}
		else
		{
			int status;
			waitpid(pid, &status, 0);
			if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
				return (0);
		}
	}
	return (1);
}


int	execute_pipes(t_tree *tree, t_env **envh, t_tool	*tool)
{
	static t_pid	*pid;
	int				pipefd[2];
	pid_t			pids[2];
	int				status[2];
	t_pid			*tmp;

	if (can_we_fork(tree))
	{
		if (tree->type != NODE_PIPE)
			return (-1);
		tool->inside_pipe = 1;
		if (pipe(pipefd) == -1)
			return (perror("Pipe error"), 1);
		pids[0] = fork();
		if (pids[0] > 0)
			pid = add_to_pids_list(pid, pids[0]);
		if (pids[0] == -1)
		{
			perror("Fork failed");
			while (pid)
			{
				kill(pid->value, SIGKILL);
				tmp = pid;
				pid = pid->next;
				free(pid);
				pid = NULL;
			}
			return (1);
		}
		if (pids[0] == 0)
		{
			(close(pipefd[0]), dup2(pipefd[1], 1));
			(close(pipefd[1]), exit(execute_tree(tree->left, envh, tool)));
		}
		tool->err = 0;
		pids[1] = fork();
		if (pids[1] == -1)
		{		perror("Fork failed");
			while (pid)
			{
				kill(pid->value, SIGKILL);
				tmp = pid;
				pid = pid->next;
				free(pid);
				pid = NULL;
			}
			return (1);
		}
		if (pids[1] > 0)
			pid = add_to_pids_list(pid, pids[1]);
		if (pids[1] == 0)
		{
			(close(pipefd[1]), dup2(pipefd[0], 0));
			(close(pipefd[0]), exit(execute_tree(tree->right, envh, tool)));
		}
		signal(SIGINT, SIG_IGN);
		(close(pipefd[0]), close(pipefd[1]));
		(waitpid(pids[0], &status[0], 0), waitpid(pids[1], &status[1], 0));
		if(WIFSIGNALED(status[0]))
			tool->signal = WTERMSIG(status[0]);
		if(WIFSIGNALED(status[1]))
		{
			tool->signal = WTERMSIG(status[1]);
			return (tool->signal + 128);
		}
		if (WIFSIGNALED(status[0]) && !WIFSIGNALED(status[1]) && WTERMSIG(status[0]) == SIGQUIT)
			tool->signal = -3;
		tool->inside_pipe = 0;
		return (WEXITSTATUS(status[1]));
	}
	return (1);
}

int	execute_tree(t_tree *tree, t_env **envh, t_tool	*tool)
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
