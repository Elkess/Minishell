/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:51:01 by melkess           #+#    #+#             */
/*   Updated: 2025/06/15 16:31:27 by melkess          ###   ########.fr       */
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

char	**struct_to_darr(t_env *envh, t_tool *tool)
{
	size_t	len;
	t_env	*tmp;
	char	**env;

	len = ft_envlen(envh);
	env = malloc((len +1) * sizeof(char *));
	if (!env)
		(free_envh(envh), exit (2));
	add_to_grbg(&tool->grbg, env);
	len = 0;
	while (envh)
	{
		if (envh->value)
		{
			env[len] = ft_strjoin(envh->key, "=", tool);
			env[len] = ft_strjoin(env[len], envh->value, tool);
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
		(print_err(NULL, path, "command not found"), exit(127)); // SHoud it be exit and free_ evnh exit YES
	if (!stat(path, &s) && S_ISDIR(s.st_mode))
	{
		if ((p && *p && !ft_strcmp(path, "..")) || !ft_strchr(path, '/'))
			(print_err(NULL, path, "command not found"),exit(127)); // SHoud it be exit and free_ evnh exit YES
		else
			(print_err(NULL, path, "is a directory"), exit(126)); // SHoud it be exit and free_ evnh exit YES
	}
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
					(perror("Execve2 Failed:"), exit(1)); // SHoud it be exit and free_ evnh ??? exit
			}
			i++;
		}
	}
}

void	execute_one(t_tree *cmd, t_env *envh, t_tool *tool)
{
	pid_t		pid;
	char		**env;
	char		**path;

	path = NULL;
	env = NULL;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (search_for_defaults(envh, "PATH"))
		path = ft_split(search_for_defaults(envh, "PATH")->value, ':', tool);
	env = struct_to_darr(envh, tool);
	pid = 0;
	if (!tool->fork)
		pid = fork();
	if (pid == -1)
		(perror("Fork1 Failed"), exit(1)); // SHoud it be exit and free_ evnh ??? exit
	if (pid == 0 || tool->fork)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		is_dir(path, cmd->cmd[0]);
		if ((!access(cmd->cmd[0], X_OK) && !path) || (!access(cmd->cmd[0], X_OK) && ft_strchr(cmd->cmd[0], '/')))
			if (execve(cmd->cmd[0], cmd->cmd, env) == -1)
				(perror("Execve1 Failed:"), exit(1));// SHoud it be exit and free_ evnh ??? exit
		exec_helper(cmd->cmd, env, tool, path);
		if (errno == 13)
			(print_err(NULL, cmd->cmd[0], strerror(errno)), exit (126));
		if (((errno == 20 || errno == 2) && ft_strchr(cmd->cmd[0], '/')) || !path)
			(print_err(NULL, cmd->cmd[0], strerror(errno)), exit (126 * (errno != 2) + 127 * (errno == 2)));
		(print_err(NULL, cmd->cmd[0], "command not found"), exit(127));// SHoud it be exit and free_ evnh ??? exit
	}
	// (free_twod(path), free_twod(env));
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
		dup2(io[0], 0);
		dup2(io[1], 1);
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

	// if (lastin && lastin->type != REDIR_HEREDOC && (!lastout ||
	// 	(lastout && lastin->index < lastout->index)))
	// {
	// 	lastin->fd = open(lastin->file, O_RDONLY);
	// 	if (lastin->fd == -1)
	// 		return (print_err(NULL, lastin->file, strerror(errno)), 1);
	// }
	// if (lastout && lastout->type == REDIR_OUT)
	// 	lastout->fd = open(lastout->file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	// else if (lastout && lastout->type == REDIR_APPEND)
	// 	lastout->fd = open(lastout->file, O_CREAT | O_APPEND | O_WRONLY, 0644);
	// if (lastout && lastout->fd == -1)
	// 	return (print_err(NULL, lastout->file, strerror(errno)), 1);
	// if (lastin && lastin->type != REDIR_HEREDOC && lastout && lastin->index > lastout->index)
	// {
	// 	lastin->fd = open(lastin->file, O_RDONLY);
	// 	if (lastin->fd == -1)
	// 		return (print_err(NULL, lastin->file, strerror(errno)), 1);
	// }
	if (lastin)
		dup2(lastin->fd, 0);
	if (lastout)
		dup2(lastout->fd, 1);
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
		// if (red != find_lastredir(tree->redirs, red->type) && red->type != REDIR_HEREDOC)
		// {
			if (red->is_ambiguous)
				return (print_err(NULL, red->file, " ambiguous redirect"), 1);
			if (red->type == REDIR_IN)
				red->fd = open(red->file, O_RDONLY);
			else if (red->type == REDIR_OUT)
				red->fd = open(red->file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
			else if (red->type == REDIR_APPEND)
				red->fd = open(red->file, O_CREAT | O_APPEND | O_WRONLY, 0644);
			if (red->fd == -1 && red->type != REDIR_HEREDOC)
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

char	*generate_file(t_redir *red, t_tool *tool)
{// USE STATIC instead 
	char	*str;
	size_t	i;

	str = NULL;
	i = 0;
	while (1)
	{
		str = ft_strjoin("/tmp/.here_doc", ft_itoa(i, tool), tool); //leaks
		red->fd = open(str, O_CREAT | O_RDWR | O_APPEND | O_EXCL, 0644);
		if (red->fd != -1)
			break ;
		i++;
	}
	return (str);
}

void	sig_herdoc(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		exit(130);
	}
}

size_t	there_is_herdoc(t_redir *red)
{
	size_t	i;

	i = 0;
	while (red)
	{
		if (red->type == REDIR_HEREDOC)
			i++;
		red = red->next;
	}
	return (i);
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

void	here_docs(t_redir *red, t_env *envh, t_tool *tool)
{
	char			*line;
	char			*lines;
	int				status;
	struct termios	orig_termios;
	pid_t			pid;
	char	buff[2];
	int				fd[2];
	size_t			n_herdocs;
	t_redir			*backup;
	int				sig;

	if (pipe(fd) == -1)
		print_err(NULL, "pipe failed :", strerror(errno));
	pid = 0;
	backup = red;
	n_herdocs = there_is_herdoc(red);
	line = NULL;
	lines = NULL;
	if (n_herdocs > 16)
		(ft_putstr_fd("minishell: maximum here-document count exceeded", 2), exit(2));
	if (n_herdocs)
		pid = fork();
	if (pid == 0 && n_herdocs)
	{
		close(fd[0]);
		signal(SIGINT, SIG_DFL);
		while (red)
		{
			if (red->type == REDIR_HEREDOC)
			{
				while (1)
				{						
					disable_echoctl(&orig_termios);
					line = readline("> ");
					if (line)
						add_to_grbg(&tool->grbg, line);
					if (!line || !ft_strcmp(line, red->file))
						break;
					line = ft_strjoin(line, "\n", tool);// TODO: need to understand how it works as in child 
					lines = ft_strjoin(lines, line, tool);
				}
				if (lines)
					write(fd[1], lines, ft_strlen(lines) +1);
				lines = NULL;
			}
			if (find_lasthd(red) == red)
				exit(0);
			red = red->next;
		}
		exit(0);
	}
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			printf("\n");
			tool->herdoc_err = 1;
		}
	}
	red = backup;
	close(fd[1]);
	while (red && !WIFSIGNALED(status))
	{
		if (red->type == REDIR_HEREDOC)
		{
			red->content = NULL;
			lines = NULL;
			while (1)
			{
				if (read(fd[0], buff, 1) <= 0)
					break ;
				if (buff[0] == '\0')
					break ;
				buff[1] = '\0';
				lines = ft_strjoin(lines, buff, tool);
			}
			red->content = lines;
		}
		red = red->next;
	}
	setup_signals();
}

int	is_builtin(t_tree *tree, char	*cmd, t_env *envh, t_tool *tool)
{
	static char	*pwd_backup;

	if (!ft_strcmp(cmd, "echo"))
		return(ft_echo(tree, tool));
	else if (!ft_strcmp(cmd, "cd"))
		return(cd(&envh, tree, &pwd_backup, tool));
	else if (!ft_strcmp(cmd, "export"))
		return(ft_export(&envh, tree, tool));
	else if (!ft_strcmp(cmd, "unset"))
		return(unset(&envh, tree->cmd));
	else if (!ft_strcmp(cmd, "env"))
		return(env(envh, tool));
	else if (!ft_strcmp(cmd, "pwd"))
		return(pwd(&pwd_backup));
	else if (!ft_strcmp(cmd, "exit"))
		return (ft_exit(tree, 0, envh, tool));
	return (-1);
}

int	execute_cmd(t_tree *tree, t_env *envh, int status, t_tool *tool)
{
	char	*cmd;
	int		data[2];

	if (tree && tree->cmd)
	{
		cmd = tree->cmd[0];
		if (!cmd)
			return (0);
		status = is_builtin(tree, cmd, envh, tool);
		if (status == -1)
		{
			execute_one(tree, envh, tool);
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
	return (status);
}

char	*expand_herdoc_content(char *line, t_tool *tool, int status)
{
	char	*var_name;
	t_env	*expand_line;
	char	*result;
	int		i;
	int j;

	i = 0;
	expand_line = NULL;
	result = NULL; 
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1])
		{
			var_name = NULL;
			i++;

			if (line[i] == '?')
			{
				result = ft_strjoin(result, ft_itoa(status, tool), tool);
				i++;
			}
			else if (line[i] == '$')
			{
				result = strjoin_char(result, '$', tool);
				i++;
			}
			else if (line[i] == '0')
			{
				result = ft_strjoin(result, "minishell", tool);
				i++;
			}
			else if (valid_char(line[i]))
			{
				j = i;
				while (line[i] && valid_char(line[i]))
					i++;

				var_name = ft_substr(line, j, i - j, tool);

				if (var_name)
				{
					expand_line = search_for_defaults(tool->envh, var_name);
					if (expand_line && expand_line->value)
						result = ft_strjoin(result, expand_line->value, tool); 
					// free(var_name);
				}
			}
			else
				result = strjoin_char(result, '$', tool);
		}
		else
		{
			result = strjoin_char(result, line[i], tool);
			i++;
		}
	}
	return (result); 
}

void	read_from_heredoc(t_redir *red, t_tool *tool, int status)
{
	char	*file;
	char	*line;
	int		fd;

	while (red)
	{
		if (red->type == REDIR_HEREDOC)
		{
			file = generate_file(red, tool);
			write(red->fd, red->content, ft_strlen(red->content));
			close(red->fd);
			red->fd = open(file, O_CREAT | O_RDWR , 0644);
			if (red->fd == -1)
				(perror("Open failed in 272:"));
			unlink(file);
			if (red->flag)
			{
				fd = open(file, O_CREAT | O_RDWR , 0644);
				if (fd == -1)
					(perror("Open failed in 469:"));
				line = get_next_line(red->fd);
				while (line)
				{
					add_to_grbg(&tool->grbg, line);
					line = expand_herdoc_content(line, tool, status);
					write(fd, line, ft_strlen(line) + 1);
					line = get_next_line(red->fd);
				}
				close(red->fd);
				close(fd);
				red->fd = open(file, O_CREAT | O_RDWR , 0644);
				if (red->fd == -1)
					(perror("Open failed in 469:"));
			}
		}
		red = red->next;
	}
}

int	executor(t_tree *tree, t_env *envh, t_tool	*tool)
{
	int		fds[2];
	int		redir_status = 0;
	int		status;
	char **expanded_cmd;

	status = tool->err;
	tool->envh = envh;
	if (!tool || !tree || tree->type != NODE_COMMAND && tree->type != NODE_PARENTHS)
		return (1);
	expanded_cmd = handel_expand(tree, status, tool);
    tree->cmd = expanded_cmd;
	expand_redir(tree, tool, status);
	ft_dup(fds, 1);
	if (tree->redirs)
	{
		read_from_heredoc(tree->redirs, tool, status);
		redir_status = ft_redir(tree);
	}
	if (!redir_status && tree->type == NODE_COMMAND)
		status = execute_cmd(tree, envh, status, tool);
	else if (!redir_status && tree->type == NODE_PARENTHS)
		status = execute_tree(tree->left, envh, tool);
	ft_dup(fds, 0);
	if (!tree->cmd && tree->type == NODE_COMMAND)
		return (redir_status);
	return (status);
}
