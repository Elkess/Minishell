/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 10:01:34 by melkess           #+#    #+#             */
/*   Updated: 2025/06/19 15:44:43 by melkess          ###   ########.fr       */
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

int	cd_helper(t_env **envh, char *cmd, char	**pwd_backup ,t_tool *tool)
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
		dprintf(2, "1{%p}\n", *pwd_backup);
		dprintf(2, "1[%s]\n", *pwd_backup);
		if (*pwd_backup && (*pwd_backup)[ft_strlen(*pwd_backup) - 1] != '/')
		{
			char *tmp = ft_strjoin_env("/", cmd, 0);
			*pwd_backup = ft_strjoin_env(*pwd_backup, tmp, 3);
		}
		else
			*pwd_backup = ft_strjoin_env(*pwd_backup, cmd, 1);
		ft_putstr_fd("5cd: error retrieving current directory: getcwd:"
		" cannot access parent directories: No such file or directory\n", 2);
		return (0);
	}
	return (0);
}

int	is_full_of_dots(char *str)
{
	size_t	i;

	i = 0;
	while (str && str[i] && str[i] == '.' && (str[i +1] == '/' || str[i +1] == '\0'))
		i++;
	if (i == ft_strlen(str))
		return (1);
	return (0);
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
		return (cd_helper(envh, cmd->cmd[0], pwd_backup, tool));
	else
	{
		DIR		*tmp;
		int		has_doubl_dots = 0;
		char	**splited_arg = ft_split(cmd->cmd[0], '/', tool);
		char	**splited_path = ft_split(*pwd_backup, '/', tool);
		char	*new_path = NULL;
		char	*new_arg = NULL;
		size_t	i;
		i = 0;
		while (splited_arg[i])
		{
			if (!ft_strcmp(splited_arg[i], ".."))
				has_doubl_dots = 1;
			i++;
		}
		i = 0;
		if (splited_arg && splited_arg[i] && splited_arg[1])
		{
			if (ft_strcmp(splited_arg[i], "."))
				new_arg = ft_strjoin("/", splited_arg[i], tool);
			i++;
			while (splited_arg[i])
			{
				if (!ft_strcmp(splited_arg[i], "..") && has_doubl_dots)
				{
					i++;
					has_doubl_dots = 0;
				}
				else if (!ft_strcmp(splited_arg[i], "."))
					i++;
				else
				{
					char *tmp = ft_strjoin("/", splited_arg[i], tool);
					new_arg = ft_strjoin(new_arg, tmp, tool);
					i++;
				}
			}
		}
		i = 0;
		while (splited_path && splited_path[i] )
		{
			new_path = ft_strjoin(new_path, "/", tool);
			tmp = opendir(ft_strjoin(new_path, splited_path[i], tool));
			if (tmp)
			{
				new_path = ft_strjoin(new_path, splited_path[i], tool);
				i++;
			}
			else
				break;
			closedir(tmp);
		}
		if (i +1 != ft_dstrlen((const char **)splited_path))
			return (print_err("4cd: ", cmd->cmd[0], strerror(errno)), 1);
		if (!is_full_of_dots(cmd->cmd[0]))
		{
			if (chdir(ft_strjoin(new_path, new_arg, tool)))
			{
				getcwd(0, 0);
				return (print_err("5cd: ", cmd->cmd[0], strerror(errno)), 1);
			}
		}
		else
		{
			if (chdir(cmd->cmd[0]))
			{
				getcwd(0, 0);
				return (print_err("6cd: ", cmd->cmd[0], strerror(errno)), 1);
			}
		}
	}
	if (search_for_defaults(*envh, "PWD"))
		*envh = edit_env(ft_strdup_env("PWD", 0), ft_strdup_env(*pwd_backup, 0), *envh, 0);
	return (0);
}
