/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:51:01 by melkess           #+#    #+#             */
/*   Updated: 2025/06/20 10:32:53 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void print_err(char *msg1, char *arg, char *msg2)
{
	int	fd = dup(1);
	dup2(2, 1);
	if (!msg1)
		printf("minishell: %s: %s\n", arg, msg2);
	else
		printf("minishell: %s%s %s\n", msg1, arg, msg2);
	dup2(fd, 1);
}

char	**struct_to_darr(t_env *envh, t_tool *tool)
{
	size_t	len;
	char	**env;

	len = ft_envlen(envh);
	env = malloc((len +1) * sizeof(char *));
	if (!env)
		(perror(""), free_envh(envh), exit (2));
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

int	is_dir(char **p, char *path)
{
	struct stat	s;

	if (!ft_strcmp(path, "."))
		(print_err(NULL, path, "5command not found"), exit(127));
	if (!stat(path, &s) && S_ISDIR(s.st_mode))
	{
		if ((p && *p && !ft_strcmp(path, "..")) || (!ft_strchr(path, '/') && p && *p))
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

int	execute_one(t_tree *cmd, t_env *envh, t_tool *tool)
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
	if (!tool->inside_pipe)
		pid = fork();
	if (pid == -1)
	{
		print_err(NULL, "Fork1 Failed", strerror(errno));
		return (1);
	}
	if (pid == 0 || tool->inside_pipe)
	{
		tool->inside_pipe = 0;
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (!is_dir(path, cmd->cmd[0]))
		{	
			if ((!access(cmd->cmd[0], X_OK) && (!path || !*path)) || (!access(cmd->cmd[0], X_OK) && ft_strchr(cmd->cmd[0], '/')))
				if (execve(cmd->cmd[0], cmd->cmd, env) == -1)
					(perror("Execve1 Failed:"), exit(1));
			exec_helper(cmd->cmd, env, tool, path);
		}
		if (errno == 13)
			(print_err(NULL, cmd->cmd[0], strerror(errno)), exit (126));
		if (((errno == 20 || errno == 2) && ft_strchr(cmd->cmd[0], '/')) || !path || !*path)
			(print_err(NULL, cmd->cmd[0], strerror(errno)), exit (126 * (errno != 2) + 127 * (errno == 2)));
		(print_err(NULL, cmd->cmd[0], "2command not found"), exit(127));
	}
	return (0);
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

int	apply_dup2(t_redir *redirs)
{
	t_redir *current = redirs;

	while (current)
	{
		if ((current->type == REDIR_IN || current->type == REDIR_HEREDOC) && !isatty(current->fd))
		{
			if (dup2(current->fd, STDIN_FILENO) == -1)
				return (-1);
		}
		else if ((current->type == REDIR_OUT || current->type == REDIR_APPEND) && !isatty(current->fd))
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

char	*generate_file(t_redir *red, t_tool *tool)
{
	char	*str;
	size_t	i;

	str = NULL;
	i = 0;
	while (1)
	{
		str = ft_strjoin("/tmp/.here_doc", ft_itoa(i, tool), tool);
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

void	here_docs(t_redir *red, t_tool *tool)
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
					line = ft_strjoin(line, "\n", tool);
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
	close(fd[0]);
	setup_signals();
}

int	is_builtin(t_tree *tree, char	*cmd, t_env **envh, t_tool *tool)
{
	static char	*pwd_backup;
	char		*tmp;


	if (!ft_strcmp(cmd, "echo"))
		return(ft_echo(tree, tool));
	else if (!ft_strcmp(cmd, "cd"))
	{

		tmp = getcwd(0,0);
		add_to_grbg(&tool->grbg, tmp);
		if (tmp)
		{
			if (pwd_backup)
				(free(pwd_backup), pwd_backup = NULL);
			pwd_backup = getcwd(0, 0);
		}
		// add_to_grbg(&tool->grbg, pwd_backup);
		return(cd(envh, tree, &pwd_backup, tool));
	}
	else if (!ft_strcmp(cmd, "export"))
		return(ft_export(envh, tree, tool));
	else if (!ft_strcmp(cmd, "unset"))
		return(unset(envh, tree->cmd));
	else if (!ft_strcmp(cmd, "env"))
		return(env(*envh, tool));
	else if (!ft_strcmp(cmd, "pwd"))
		return(pwd(pwd_backup));
	else if (!ft_strcmp(cmd, "exit"))
		return (ft_exit(tree, 0, *envh, tool));
	return (-1);
}

int	execute_cmd(t_tree *tree, t_env **envh, int status, t_tool *tool)
{
	char		*cmd;

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
				(perror("Open failed in 524:"));
			unlink(file);
			if (red->flag)
			{
				fd = open(file, O_CREAT | O_RDWR , 0644);
				if (fd == -1)
					(perror("Open failed in 530:"));
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
				unlink(file);
			}
		}
		red = red->next;
	}
}

int	executor(t_tree *tree, t_env **envh, t_tool	*tool)
{
	int		fds[2];
	int		redir_status = 0;
	int		status;
	char **expanded_cmd;

	status = tool->err;
	tool->envh = *envh;
	if (!tool || !tree || (tree->type != NODE_COMMAND && tree->type != NODE_PARENTHS))
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
	if (redir_status)
		return (redir_status);
	return (status);
}
