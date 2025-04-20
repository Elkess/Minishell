/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:51:01 by melkess           #+#    #+#             */
/*   Updated: 2025/04/20 09:12:42 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor.h"

char	**struct_to_darr(t_env *envh)
{
	size_t	len;
	t_env	*tmp;
	char	**env;

	len = 0;
	tmp = envh;
	while (tmp)
	{
		len++;
		tmp = tmp->next;
	}
	env = malloc((len +1) * sizeof(char *));
	if (!env)
		exit (1);
	len = 0;
	while (envh)
	{
		if (envh->value)
		{
			env[len] = ft_strjoin(envh->key, "=");
			env[len] = ft_strjoin(env[len], envh->value);
			len++;
		}
		envh = envh->next;
	}
	return (env[len] = NULL, env);
}

void	execute_one(t_tree *cmd, t_env *envh)
{
	pid_t 	fd = fork();
	char	**env;
	char	**path;
	size_t	i;

	if (cmd->redirs)
	i = 0;
	env = struct_to_darr(envh);
	if (fd == -1)
		(perror("Fork Failed:"), exit(1));
	if (fd == 0)
	{
		if (!access(cmd->cmd[0], X_OK))
		{
			if (execve(cmd->cmd[0], cmd->cmd, env) == -1)
				(perror("Execve1 Failed:"), exit(1));
		}
		else if (!ft_strchr(cmd->cmd[0], '/'))
		{
			// execute_with_path()
			path = ft_split(search_for_defaults(envh, "PATH")->value, ':');
			while (path && path[i])
			{
				path[i] = ft_strjoin(path[i], "/");
				path[i] = ft_strjoin(path[i], cmd->cmd[0]);
				if (!access(path[i], X_OK))
				{
					if (execve(path[i], cmd->cmd, env) == -1)
						(perror("Execve2 Failed:"), exit(1));		
				}
				i++;
			}
		}
		(printf("minishell: command not found: %s\n", cmd->cmd[0]), exit(1));
	}
}

void	ft_dup(int *io, int flag)
{
	if (flag)
	{
		io[0] = dup(0);
		io[1] = dup(1);
		if (io[0] == -1 || io[1] == -1)
		{
			(close(io[0]), close(io[1]));
		}
	}
	else
	{
		if (dup2(io[0], 0) == -1 || dup2(io[1], 1) == -1)
		{
			(close(io[0]), close(io[1]));
		}
		(close(io[0]), close(io[1]));
	}
}

void	ft_redir(t_tree *tree, int flag, int *fds)
{
	int		fd;
	int		trunc;
	int		create;
	size_t	count;
	t_redir	*red;

	count = 0;
	red = tree->redirs;
	trunc = flag * O_TRUNC;
	create = flag * O_CREAT;
	while (red)
	{
		if (red->type == flag)
			count++;
		red = red->next;
	}
	while (tree->redirs)
	{
		if (tree->redirs->type != flag)
			tree->redirs = tree->redirs->next;
		else if (tree->redirs->type == flag && count > 1)
		{
			if (flag)
				fd = open(tree->redirs->file, create | trunc | O_RDWR);
			else
				fd = open(tree->redirs->file, create | trunc | O_RDWR);
			close(fd);
			count--;
		}
		else
			fd = open(tree->redirs->file, create | trunc | O_RDWR);
		tree->redirs = tree->redirs->next;
	}
	ft_dup(fds, 1);
	// printf("%d\n", flag);
	dup2(fd, flag);
}

int	executor(t_tree *tree,t_env	*envh)
{
	int		status;
	int		fds[2];
	size_t	i = 0;

	if (!tree || !tree->cmd)
		exit (999);
	// while (tree->cmd[i])
	// 	puts(tree->cmd[i++]);
	// // FIXME: SHLVL
	// if (!tree || tree->type != NODE_COMMAND)
	// 	{printf("NO YEEEET !!!");}
	// if (tree->redirs)
	// {
	// 	if (tree->redirs->type == REDIR_IN )
	// 		ft_redir(tree, 0, fds);
	// 	else if (tree->redirs->type == REDIR_OUT)
	// 		ft_redir(tree, 1, fds);
	// 	else 
	// 		{puts("NOT YEEEET A!"); return (1);}
	// }
	if (tree->cmd && !ft_strcmp(tree->cmd[0], "echo"))
		echo(tree);
	else if (tree->cmd && !ft_strcmp(tree->cmd[0], "env"))
		env(envh);
	else if (tree->cmd && !ft_strcmp(tree->cmd[0], "cd"))
		cd(&envh, tree);
	else if (tree->cmd && !ft_strcmp(tree->cmd[0], "export"))
		export(&envh, tree);
	else if (tree->cmd && !ft_strcmp(tree->cmd[0], "unset"))
		unset(&envh, tree->cmd);
	else if (tree->cmd && !ft_strcmp(tree->cmd[0], "exit"))
		ft_exit(tree);
	else if (tree->cmd && !ft_strcmp(tree->cmd[0], "pwd"))
		pwd(1);
	else
	{
		execute_one(tree, envh);
		waitpid(0, &status, 0);	
	}
	// ft_dup(fds, 0);
	// t_env *tmp;
	// while(envh)
	// {
	// 	tmp = envh;
	// 	free(tmp->key);
	// 	free(tmp->value);
	// 	envh = envh->next;
	// 	free(tmp);
	// }
	// free(tree);
	// while (envh)
	// {
	// 	printf("key: %s => %s\n", envh->key, envh->value);
	// 	envh = envh->next;
	// }	
	return 0;
}