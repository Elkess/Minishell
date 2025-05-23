/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:51:01 by melkess           #+#    #+#             */
/*   Updated: 2025/05/22 08:53:56 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void print_err(char *msg1, char *arg, char *msg2)
{
	// store backup first
	int	fd = dup(1);
	dup2(2, 1);
	if (!msg1)
		printf("minishell: %s: %s\n", arg, msg2);
	else
		printf("minishell: %s%s: %s\n", msg1, arg, msg2);
	dup2(fd, 1);
}

char	**struct_to_darr(t_env *envh)
{
	size_t	len;
	t_env	*tmp;
	char	**env;

	len = ft_envlen(envh);
	env = malloc((len +1) * sizeof(char *));
	if (!env)
		(free_envh(envh), exit (1));
	len = 0;
	while (envh)
	{
		if (envh->value)
		{
			env[len] = ft_strjoin(envh->key, "=", 0);
			// free tmp of before env[len]
			env[len] = ft_strjoin(env[len], envh->value, 1);
			len++;
		}
		envh = envh->next;
	}
	env[len] = NULL;
	return (env);
}

void	is_dir(char **p, char *path)
{
	struct stat	s;

	if (!ft_strcmp(path, "."))
		(print_err(NULL, path, ": command not found"), exit(127)); // SHoud it be exit and free_ evnh ??? exit YES
	if (!stat(path, &s) && S_ISDIR(s.st_mode))
	{
		if ((p && *p && !ft_strcmp(path, "..")) || !ft_strchr(path, '/'))
			(print_err(NULL, path, ": command not found"),exit(127)); // SHoud it be exit and free_ evnh ??? exit YES
		else
			(print_err(NULL, path, ": is a directory"), exit(126)); // SHoud it be exit and free_ evnh ??? exit YES
	}
}

void	exec_helper(char **cmd, char **env, t_env *envh, char **path)
{
	size_t	i;
	char	*new_path;

	new_path = NULL;
	i = 0;
	if (!ft_strchr(cmd[0], '/'))
	{
		while (path && path[i])
		{
			new_path = ft_strjoin(path[i], "/", 0);
			// free before path[i] in tmp
			new_path = ft_strjoin(new_path, cmd[0], 1);
			if (!access(new_path, X_OK))
			{
				if (execve(new_path, cmd, env) == -1)
					(perror("Execve2 Failed:"), exit(1)); // SHoud it be exit and free_ evnh ??? exit
			}
			free(new_path);
			i++;
		}
	}
}

void	execute_one(t_tree *cmd, t_env *envh)
{
	pid_t		fd = fork();
	char		**env;
	char		**path;

	path = NULL;
	env = NULL;
	if (search_for_defaults(envh, "PATH"))
		path = ft_split(search_for_defaults(envh, "PATH")->value, ':');
	env = struct_to_darr(envh);
	if (fd == -1)
		(perror("Fork1 Failed"), exit(1)); // SHoud it be exit and free_ evnh ??? exit
	if (fd == 0)
	{
		is_dir(path, cmd->cmd[0]);
		if ((!access(cmd->cmd[0], X_OK)))
		{
			if (execve(cmd->cmd[0], cmd->cmd, env) == -1)
				(perror("Execve1 Failed:"), exit(1));// SHoud it be exit and free_ evnh ??? exit
		}
		exec_helper(cmd->cmd, env, envh, path);
		if (((errno == 20 || errno == 13 || errno == 2) && ft_strchr(cmd->cmd[0], '/')) || !path)
			(print_err(NULL, cmd->cmd[0], strerror(errno)), exit (126 * (errno != 2) + 127 * (errno == 2)));
		(print_err(NULL, cmd->cmd[0], ": command not found"), exit(127));// SHoud it be exit and free_ evnh ??? exit
	}
	(free_twod(path), free_twod(env));
}

void	ft_dup(int *io, int flag)
{
	if (flag)
	{
		io[0] = dup(0);
		io[1] = dup(1);
		if (io[0] == -1 || io[1] == -1)
			(close(io[0]), close(io[1]));
	}
	else
	{
		if (dup2(io[0], 0) == -1 || dup2(io[1], 1) == -1)
			(close(io[0]), close(io[1]));
		(close(io[0]), close(io[1]));
	}
}

t_redir	*find_lastredir(t_redir *redirs, t_redir_type type)
{
	t_redir	*last;

	last = NULL;
	if (type == REDIR_OUT || type == REDIR_APPEND)
	{
		while (redirs)
		{
			if (redirs->type == REDIR_OUT || redirs->type == REDIR_APPEND)
				last = redirs;
			redirs = redirs->next;
		}
	}
	if (type == REDIR_IN || type == REDIR_HEREDOC)
	{
		while (redirs)
		{
			if (redirs->type == REDIR_IN || redirs->type == REDIR_HEREDOC)
				last = redirs;
			redirs = redirs->next;
		}
	}
	return (last);
}

int	handle_lastredir(t_redir *redirs)
{
	t_redir	*lastin;
	t_redir	*lastout;
	lastin = find_lastredir(redirs, REDIR_IN);
	lastout = find_lastredir(redirs, REDIR_OUT);

	if (lastin && lastin->type != REDIR_HEREDOC && (!lastout ||
		(lastout && lastin->index < lastout->index)))
	{
		lastin->fd = open(lastin->file, O_RDONLY);
		if (lastin->fd == -1)
			return (print_err(NULL, lastin->file, strerror(errno)), 1);
	}
	if (lastout && lastout->type == REDIR_OUT)
		lastout->fd = open(lastout->file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	else if (lastout && lastout->type == REDIR_APPEND)
		lastout->fd = open(lastout->file, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (lastout && lastout->fd == -1)
		return (print_err(NULL, lastout->file, strerror(errno)), 1);
	if (lastin && lastin->type != REDIR_HEREDOC && lastout && lastin->index > lastout->index)
	{
		lastin->fd = open(lastin->file, O_RDONLY);
		if (lastin->fd == -1)
			return (print_err(NULL, lastin->file, strerror(errno)), 1);
	}
	if (lastin)
		dup2(lastin->fd, 0);
	if (lastout) 
		dup2(lastout->fd, 1);
	return (0);
}

int	ft_redir(t_tree *tree)
{
	t_redir	*red;

	red = tree->redirs;
	while (red)
	{
		// if (red != find_lastredir(tree->redirs, red->type) && red->type != REDIR_HEREDOC)
		// {
			if (red->type == REDIR_IN)
				red->fd = open(red->file, O_RDONLY);
			else if (red->type == REDIR_OUT)
				red->fd = open(red->file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
			else if (red->type == REDIR_APPEND)
				red->fd = open(red->file, O_CREAT | O_APPEND | O_WRONLY, 0644);
			if (red->fd == -1)
			{
				print_err(NULL, red->file, strerror(errno));
				return (1);
			}
		// }
		if (red != find_lastredir(tree->redirs, REDIR_OUT) && // opened fd
			red != find_lastredir(tree->redirs, REDIR_IN))
			close(red->fd);
		red = red->next;
	}
	return (handle_lastredir(tree->redirs));
}

char	*generate_file(t_redir *red)
{
	char	*str;
	size_t	i;

	str = NULL;
	i = 0;
	while (1)
	{
		free (str);
		str = ft_strjoin("/tmp/.here_doc", ft_itoa(i), 2); //leaks
		if (access(str, F_OK) != 0)
		{
			red->fd = open(str, O_CREAT | O_RDWR | O_APPEND);
			if (red->fd == -1)
				(perror("Open failed in << :"),free(str), exit (1));// SHoud it be exit and free_ evnh ??? exit
			break ;
		}
		i++;
	}
	return (str);
}

void	here_docs(t_redir *red, t_env *envh)
{
	char	*line;
	char	*file;

	line = NULL;
	while (red)
	{
		if (red->type == REDIR_HEREDOC)
		{
			file = generate_file(red);
			while (1)
			{
				line = readline("> ");
				if (!line)
					break ;
				if (!ft_strcmp(line, red->file))
				{
					free(line);
					break ;
				}
				// if (red->flag == 1)
				// {
				// 	line = heredoc_expand(line);
				// }
				line = ft_strjoin(line, "\n", 1);
				write(red->fd, line, ft_strlen(line));
				free(line);
			}
			close(red->fd);
			///
			red->fd = open(file, O_CREAT | O_RDWR | O_APPEND);
			// read it and expand each line
			if (red->fd == -1)
				(perror("Open failed in 272:"), free_envh(envh),free(file), exit (1)); // add free file
			unlink(file);
			free(file);
		}
		red = red->next;
	}
}

t_redir	*find_lasthd(t_redir *redirs)
{
	t_redir	*last;

	last = NULL;
	while (redirs)
	{
		if (redirs->type == REDIR_HEREDOC)
			last = redirs;
		redirs = redirs->next;
	}
	return (last);
}

int	is_builtin(t_tree *tree, char	*cmd, t_env *envh)
{
	static char	*pwd_backup;

	if (!ft_strcmp(cmd, "echo"))
		return(echo(tree));
	else if (!ft_strcmp(cmd, "cd"))
		return(cd(&envh, tree, &pwd_backup));
	else if (!ft_strcmp(cmd, "export"))
		return(ft_export(&envh, tree));
	else if (!ft_strcmp(cmd, "unset"))
		return(unset(&envh, tree->cmd));
	else if (!ft_strcmp(cmd, "env"))
		return(env(envh));
	else if (!ft_strcmp(cmd, "pwd"))
		return(pwd(&pwd_backup));
	else if (!ft_strcmp(cmd, "exit"))
		ft_exit(tree, 0, envh);
	return (-1);
}

int	execute_cmd(t_tree *tree, t_env *envh, int status)
{
	char	*cmd;
	if (tree && tree->cmd)
	{

		cmd = tree->cmd[0];
		if (!cmd)
			return (0);
		status = is_builtin(tree, cmd, envh);
		if (status == -1)
		{
			(execute_one(tree, envh), waitpid(0, &status, 0));
			status = WEXITSTATUS(status);
		}
	}
	return (status);
}

int	executor(t_tree *tree, t_env *envh)
{
	// TODO: handle tty && signals 
	int		fds[2];
	int		redir_status = 0;
	int		status;
	t_redir	lasthd;

	status = 0;
	if (!tree && tree->type != NODE_COMMAND && tree->type != NODE_PARENTHS)
		return (1);
	ft_dup(fds, 1);
	if (tree->redirs)
	{
		// lasthd = find_lasthd(tree->redirs);
		// TODO: 2 handle herdoc properly this is wrong
		// if (lasthd)
		// 	here_docs(tree->redirs);
		redir_status = ft_redir(tree);
	}
	if (!redir_status && tree->type == NODE_COMMAND)
		status = execute_cmd(tree, envh, status);
	else if (!redir_status && tree->type == NODE_PARENTHS)
		status = execute_tree(tree->left, envh);
	ft_dup(fds, 0);
	if (!tree->cmd)
		return (redir_status);
	return (status);
}
