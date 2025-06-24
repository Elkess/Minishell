/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 18:41:24 by melkess           #+#    #+#             */
/*   Updated: 2025/06/23 17:01:48 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*rebuild_arg(char **splited_arg, t_tool *tool, int has_doubl_dots)
{
	char	*tmp;
	size_t	i;

	1 && (tool->new_arg = NULL, i = 0, tool->dot_counter = 0);
	while (splited_arg[i])
	{
		if (!ft_strcmp(splited_arg[i], "..") && has_doubl_dots)
		{
			i++;
			has_doubl_dots = 0;
		}
		else if (!ft_strcmp(splited_arg[i], "."))
			1 && (i++, tool->dot_counter++);
		else
		{
			tmp = ft_strjoin("/", splited_arg[i], tool);
			tool->new_arg = ft_strjoin(tool->new_arg, tmp, tool);
			i++;
		}
	}
	tool->dot_counter = (tool->dot_counter == i) * 1;
	return (tool->new_arg);
}

char	*rebuild_path(char **splited_path, t_tool *tool, size_t *i)
{
	DIR		*tmp;
	char	*new_path;

	new_path = NULL;
	while (splited_path && splited_path[*i])
	{
		new_path = ft_strjoin(new_path, "/", tool);
		tmp = opendir(ft_strjoin(new_path, splited_path[*i], tool));
		if (tmp)
		{
			new_path = ft_strjoin(new_path, splited_path[*i], tool);
			(*i)++;
		}
		else
			break ;
		closedir(tmp);
	}
	return (new_path);
}

int	handle_chdir(t_env **envh, t_tree *cmd, char **pwd_backup, t_tool *tool)
{
	if (!tool->dot_counter)
	{
		if (chdir(ft_strjoin(tool->new_path, tool->new_arg, tool)))
			return (print_err("cd: ", cmd->cmd[0], strerror(errno)), 1);
		up_key("OLDPWD", *pwd_backup, envh);
		free(*pwd_backup);
		*pwd_backup = getcwd(0, 0);
		up_key("PWD", *pwd_backup, envh);
	}
	else
	{
		if (chdir(cmd->cmd[0]))
			return (print_err("cd: ", cmd->cmd[0], strerror(errno)), 1);
		up_key("OLDPWD", *pwd_backup, envh);
		free(*pwd_backup);
		*pwd_backup = getcwd(0, 0);
		up_key("PWD", *pwd_backup, envh);
	}
	return (0);
}

int	check_double_dots(char **splited_arg)
{
	size_t	i ;

	i = 0;
	while (splited_arg[i])
	{
		if (!ft_strcmp(splited_arg[i], ".."))
			return (1);
		i++;
	}
	return (0);
}

int	cd_complex(t_env **envh, t_tree *cmd, char **pwd_backup, t_tool *tool)
{
	int		has_doubl_dots;
	char	**splited_arg;
	char	**splited_path;
	size_t	i;

	i = 0;
	splited_arg = ft_split(cmd->cmd[0], '/', tool);
	splited_path = ft_split(*pwd_backup, '/', tool);
	has_doubl_dots = check_double_dots(splited_arg);
	tool->new_arg = rebuild_arg(splited_arg, tool, has_doubl_dots);
	if ((!ft_strcmp(splited_arg[0], cmd->cmd[0])
			&& !splited_arg[1]) || tool->dot_counter)
		tool->new_path = NULL;
	else
		tool->new_path = rebuild_path(splited_path, tool, &i);
	if (!ft_strcmp(cmd->cmd[0], ".."))
		tool->new_path = rebuild_path(splited_path, tool, &i);
	if (i + 1 != ft_dstrlen((const char **)splited_path))
		return (print_err("cd: ", cmd->cmd[0], strerror(errno)), 1);
	return (handle_chdir(envh, cmd, pwd_backup, tool));
}
