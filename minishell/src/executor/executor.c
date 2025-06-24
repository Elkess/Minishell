/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:51:01 by melkess           #+#    #+#             */
/*   Updated: 2025/06/24 11:43:47 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	apply_dup2(t_redir *redirs)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
		if ((current->type == REDIR_IN || current->type == REDIR_HEREDOC)
			&& !isatty(current->fd))
		{
			if (dup2(current->fd, STDIN_FILENO) == -1)
				return (-1);
		}
		else if ((current->type == REDIR_OUT || current->type == REDIR_APPEND)
			&& !isatty(current->fd))
		{
			if (dup2(current->fd, STDOUT_FILENO) == -1)
				return (-1);
		}
		close(current->fd);
		current = current->next;
	}
	return (0);
}

int	ft_redir(t_tree *tree)
{
	t_redir	*red;

	if (!tree || !tree->redirs)
		return (0);
	red = tree->redirs;
	while (red)
	{
		if (red->is_ambiguous)
			return (print_err(NULL, red->file, " ambiguous redirect"), 1);
		if (red->type == REDIR_IN)
			red->fd = open(red->file, O_RDONLY);
		else if (red->type == REDIR_OUT)
			red->fd = open(red->file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
		else if (red->type == REDIR_APPEND)
			red->fd = open(red->file, O_CREAT | O_APPEND | O_WRONLY, 0644);
		if (red->fd == -1 && red->type != REDIR_HEREDOC)
			return (print_err(NULL, red->file, strerror(errno)), 1);
		red = red->next;
	}
	return (apply_dup2(tree->redirs));
}

int	is_builtin(t_tree *tree, char	*cmd, t_env **envh, t_tool *tool)
{
	if (!ft_strcmp(cmd, "echo"))
		return (ft_echo(tree, tool));
	else if (!ft_strcmp(cmd, "cd"))
	{
		tool->tmp = getcwd(0, 0);
		if (tool->tmp)
		{
			if (tool->pwd_backup)
				(free(tool->pwd_backup),
					tool->pwd_backup = NULL);
			tool->pwd_backup = tool->tmp;
		}
		return (cd(envh, tree, &tool->pwd_backup, tool));
	}
	else if (!ft_strcmp(cmd, "export"))
		return (ft_export(envh, tree, tool));
	else if (!ft_strcmp(cmd, "unset"))
		return (unset(envh, tree->cmd));
	else if (!ft_strcmp(cmd, "env"))
		return (env(*envh, tool));
	else if (!ft_strcmp(cmd, "pwd"))
		return (pwd(tool->pwd_backup));
	else if (!ft_strcmp(cmd, "exit"))
		return (ft_exit(tree, 0, *envh, tool));
	return (-1);
}

int	execute_cmd(t_tree *tree, t_env **envh, int status, t_tool *tool)
{
	char	*cmd;

	if (tree && tree->cmd)
	{
		cmd = tree->cmd[0];
		if (!cmd)
			return (0);
		status = is_builtin(tree, cmd, envh, tool);
		if (status == -1)
		{
			status = execute_one(tree, *envh, tool);
			waitpid(0, &status, 0);
			if (WIFEXITED(status))
				status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				tool->signal = WTERMSIG(status);
				status = WTERMSIG(status) + 128;
			}
		}
	}
	else
		status = 0;
	return (status);
}

int	executor(t_tree *tree, t_env **envh, t_tool	*tool)
{
	int		fds[2];
	int		redir_status;
	int		status;
	char	**expanded_cmd;

	1 && (redir_status = 0, status = tool->err, tool->envh = *envh);
	if (!tool || !tree || (tree->type != 0 && tree->type != 4))
		return (1);
	expanded_cmd = handel_expand(tree, status, tool);
	tree->cmd = expanded_cmd;
	(expand_redir(tree, tool, status), ft_dup(fds, 1));
	if (tree->redirs)
	{
		read_from_heredoc(tree->redirs, tool, status);
		redir_status = ft_redir(tree);
	}
	if (!redir_status && tree->type == 0)
		status = execute_cmd(tree, envh, status, tool);
	else if (!redir_status && tree->type == NODE_PARENTHS)
		status = execute_tree(tree->left, envh, tool);
	ft_dup(fds, 0);
	if (redir_status)
		return (redir_status);
	return (status);
}
