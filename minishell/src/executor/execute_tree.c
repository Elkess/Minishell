/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 08:39:55 by melkess           #+#    #+#             */
/*   Updated: 2025/06/12 15:22:58 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void static handle_ctrbackslash(int sig)
{
	write(1, "^\\Quit: 3\n", 11);
	exit(131);
}

typedef struct s_pid
{
	int		value;
	struct s_pid *next;
} t_pid;


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

int	execute_pipes(t_tree *tree, t_env *envh, t_tool	*tool)
{
	static t_pid	*pid;
	int				pipefd[2];
	pid_t			pids[2];
	int				status[2];
	int				exit_stat;

	if (tree->type != NODE_PIPE)
		return (-1);
	tool->fork = 1;
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
			pid = pid->next;
		}
		return (1);
	}
	if (pids[0] == 0)
	{
		(close(pipefd[0]), dup2(pipefd[1], 1));
		(close(pipefd[1]), exit(execute_tree(tree->left, envh, tool))); // SHoud it be exit and free_ evnh ???
	}
	tool->err = 0;
	pids[1] = fork();
	if (pids[1] == -1)
	{
		perror("Fork failed");
		while (pid)
		{
			kill(pid->value, SIGKILL);
			pid = pid->next;
		}
		return (1);
	}
	if (pids[1] > 0)
		pid = add_to_pids_list(pid, pids[1]);
	if (pids[1] == 0)
	{
		(close(pipefd[1]), dup2(pipefd[0], 0));
		(close(pipefd[0]), exit(execute_tree(tree->right, envh, tool))); // SHoud it be exit and free_ evnh
	}
	signal(SIGINT, SIG_IGN);
	(waitpid(pids[0], &status[0], 0), waitpid(pids[1], &status[1], 0));
	(close(pipefd[0]), close(pipefd[1]));
	if(WIFSIGNALED(status[0]))
		tool->signal = WTERMSIG(status[0]);
	if(WIFSIGNALED(status[1]))
	{
		tool->signal = WTERMSIG(status[1]);
		return (tool->signal + 128);
	}
	if (WIFSIGNALED(status[0]) && !WIFSIGNALED(status[1]) && WTERMSIG(status[0]) == SIGQUIT)
		tool->signal = -3;
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
	if (tree->type == NODE_PIPE)
		return (execute_pipes(tree, envh, tool));
	return (status);
}
