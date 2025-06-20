/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:46:03 by melkess           #+#    #+#             */
/*   Updated: 2025/06/20 18:07:29 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_dir(char **p, char *path)
{
	struct stat	s;

	if (!ft_strcmp(path, "."))
		(print_err(NULL, path, "5command not found"), exit(127));
	if (!stat(path, &s) && S_ISDIR(s.st_mode))
	{
		if ((p && *p && !ft_strcmp(path, ".."))
			|| (!ft_strchr(path, '/') && p && *p))
			(print_err(NULL, path, "6command not found"), exit(127));
		else if (!p || !*p || ft_strchr(path, '/'))
			(print_err(NULL, path, "is a directory"), exit(126));
		return (1);
	}
	return (0);
}

void	exec_helper(char **cmd, char **env, t_tool *tool, char **path)
{
	size_t	i;
	char	*new_path;

	new_path = NULL;
	i = 0;
	if (!ft_strchr(cmd[0], '/') && cmd[0] && *cmd[0])
	{
		while (path && path[i])
		{
			new_path = ft_strjoin(path[i], "/", tool);
			new_path = ft_strjoin(new_path, cmd[0], tool);
			if (!access(new_path, X_OK))
			{
				if (execve(new_path, cmd, env) == -1)
					(perror("Execve2 Failed:"), exit(1));
			}
			i++;
		}
	}
}

void	handle_err(t_tree *cmd, t_tool *tool)
{
	if (errno == 13)
		(print_err(NULL, cmd->cmd[0], strerror(errno)), exit (126));
	if (((errno == 20 || errno == 2) && ft_strchr(cmd->cmd[0], '/'))
		|| !tool->path || !*tool->path)
		(print_err(NULL, cmd->cmd[0], strerror(errno)),
			exit (126 * (errno != 2) + 127 * (errno == 2)));
	(print_err(NULL, cmd->cmd[0], "command not found"), exit(127));
}

int	execute_one(t_tree *cmd, t_env *envh, t_tool *tool)
{
	pid_t		pid;
	char		**env;

	1 && (tool->path = NULL, env = NULL,
		signal(SIGINT, SIG_IGN), signal(SIGQUIT, SIG_IGN));
	if (sh_env(envh, "PATH"))
		tool->path = ft_split(sh_env(envh, "PATH")->value, ':', tool);
	1 && (env = struct_to_darr(envh, tool), pid = fork());
	if (pid == -1)
		return (print_err(NULL, "Fork Failed", strerror(errno)), 1);
	if (pid == 0)
	{
		tool->inside_pipe = 0;
		(signal(SIGINT, SIG_DFL), signal(SIGQUIT, SIG_DFL));
		if (!is_dir(tool->path, cmd->cmd[0]))
		{
			if ((!access(cmd->cmd[0], X_OK) && (!tool->path || !*tool->path))
				|| (!access(cmd->cmd[0], X_OK) && ft_strchr(cmd->cmd[0], '/')))
				if (execve(cmd->cmd[0], cmd->cmd, env) == -1)
					(perror("Execve Failed:"), exit(1));
			exec_helper(cmd->cmd, env, tool, tool->path);
		}
		handle_err(cmd, tool);
	}
	return (0);
}
