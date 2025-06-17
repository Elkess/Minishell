/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 10:01:34 by melkess           #+#    #+#             */
/*   Updated: 2025/06/17 13:38:35 by melkess          ###   ########.fr       */
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
		free(*pwd_backup);
		*pwd_backup = getcwd(0, 0);
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
		free(*pwd_backup);
		*pwd_backup = dir;
	}
	if (!chdir(cmd->cmd[0]))
		cd_helper(envh, cmd->cmd[0], pwd_backup, tool);
	else
		return (print_err("cd: ", cmd->cmd[0], strerror(errno)), 1);
	if (search_for_defaults(*envh, "PWD"))
		*envh = edit_env(ft_strdup_env("PWD", 0), ft_strdup_env(*pwd_backup, 0), *envh, 0);
	return (0);
}