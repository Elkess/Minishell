/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 10:01:34 by melkess           #+#    #+#             */
/*   Updated: 2025/06/17 23:52:24 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_home_dir(t_env *envh)
{
	char	*s;

	s = NULL;
	if (search_for_defaults(envh, "HOME"))
	{
		s = search_for_defaults(envh, "HOME")->value;
		if (!s)
		{
			printf("minishell: HOME not set\n");
			return (NULL);
		}
	}
	return (s);
}

void	cd_helper(t_env **envh, char *cmd, char	**pwd_backup ,t_tool *tool)
{
	if (search_for_defaults(*envh, "OLDPWD"))
		*envh = edit_env(ft_strdup_env("OLDPWD", 0), ft_strdup_env(*pwd_backup, 0), *envh, 0);
	if (tool->flag)
	{
		*pwd_backup = getcwd(0, 0);
		add_to_grbg(&tool->grbg, *pwd_backup);
	}
	else
	{
		if (*pwd_backup && (*pwd_backup)[ft_strlen(*pwd_backup) -1] != '/')
			*pwd_backup = ft_strjoin(*pwd_backup, ft_strjoin("/", cmd, tool), tool);
		else
			*pwd_backup = ft_strjoin(*pwd_backup, cmd, tool);
		ft_putstr_fd("cd: error retrieving current directory: getcwd:"
		" cannot access parent directories: No such file or directory\n", 2);
	}
}

int	cd(t_env **envh, t_tree *cmd, char	**pwd_backup, t_tool *tool)
{
	char	*dir;

	tool->flag = 0;
	cmd->cmd++;
	if (!cmd->cmd[0])
	{
		cmd->cmd[0] = get_home_dir(*envh);
		if (!cmd->cmd[0])
			return (1);
	}
	dir = getcwd(0, 0);
	if (dir)
	{
		tool->flag = 1;
		*pwd_backup = dir;
		add_to_grbg(&tool->grbg, *pwd_backup);
		
	}
	if (!chdir(cmd->cmd[0]))
		cd_helper(envh, cmd->cmd[0], pwd_backup, tool);
	else
	{
		size_t	counter = 0;
		DIR		*tmp;
		char	**splited_arg = ft_split(cmd->cmd[0], '/', tool);
		char	**splited_path = ft_split(*pwd_backup, '/', tool);
		char	*new_path = NULL;
		char	*new_arg = NULL;
		size_t	i;
		i = 0;
		if (splited_arg && splited_arg[i] && splited_arg[1])
		{
			new_arg = ft_strjoin("/", splited_arg[i], tool);
			i++;
			while (splited_arg[i +1])
			{
				if (!ft_strcmp(splited_arg[i], "."))
					i++;
				else
				{
					char *tmp = ft_strjoin("/", splited_arg[i], tool);
					char *joined = ft_strjoin(new_arg, tmp, tool);
					new_arg = joined;
					i++;
				}
			}
		}
		i = 0;
		while (splited_path && splited_path[i] )
		{
			new_path = ft_strjoin(new_path, "/", tool);
			new_path = ft_strjoin(new_path, splited_path[i], tool);
			tmp = opendir(new_path);
			if (tmp)
				i++;
			else
				break;
			closedir(tmp);
		}
		new_path = NULL;
		if (i +1 != ft_dstrlen((const char **)splited_path))
			return (print_err("4cd: ", cmd->cmd[0], "Permission denied"), 1);
		if (!ft_strcmp(splited_arg[ft_dstrlen((const char **)splited_arg) -1], ".."))
		{
			i = 0;
			new_path = ft_strjoin("/", splited_path[i], tool);
			i++;
			while (splited_path[i + 1])
			{
				char *tmp = ft_strjoin("/", splited_path[i], tool);
				char *joined = ft_strjoin(new_path, tmp, tool);
				new_path = joined;
				i++;
			}
		}
		if (chdir(ft_strjoin(new_path, new_arg, tool)))
			return (print_err("3cd: ", cmd->cmd[0], strerror(errno)), 1);
	}
	if (search_for_defaults(*envh, "PWD"))
		*envh = edit_env(ft_strdup_env("PWD", 0), ft_strdup_env(*pwd_backup, 0), *envh, 0);
	return (0);
}
