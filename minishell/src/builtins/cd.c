/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 10:01:34 by melkess           #+#    #+#             */
/*   Updated: 2025/06/21 18:05:30 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_home_dir(t_env *envh, t_tool *tool)
{
	char	*s;

	s = NULL;
	if (sh_env(envh, "HOME"))
	{
		s = sh_env(envh, "HOME")->value;
		if (s)
			if (chdir(s) < 0)
				tool->home = 1;
	}
	else
		ft_putstr_fd("minishell: HOME not set\n", 2);
	return (s);
}

int	cd_helper(t_env **envh, char *cmd, char	**pwd_backup, t_tool *tool)
{
	char	*tmp;

	tmp = NULL;
	if (sh_env(*envh, "OLDPWD"))
		*envh = edit_env(ft_strdup_env("OLDPWD", 0),
				ft_strdup_env(*pwd_backup, 0), *envh, 0);
	if (tool->flag)
		1 && (free(*pwd_backup), *pwd_backup = getcwd(0, 0));
	else
	{
		if (*pwd_backup && (*pwd_backup)[ft_strlen(*pwd_backup) - 1] != '/')
		{
			tmp = ft_strjoin_env("/", cmd, 0);
			*pwd_backup = ft_strjoin_env(*pwd_backup, tmp, 3);
		}
		else
			*pwd_backup = ft_strjoin_env(*pwd_backup, cmd, 1);
		tmp = getcwd(0, 0);
		if (!tmp)
			ft_putstr_fd("cd: error retrieving current directory: getcwd:" \
		" cannot access parent directories: No such file or directory\n", 2);
		return (free(tmp), 0);
	}
	return (0);
}

int	cd(t_env **envh, t_tree *cmd, char **pwd_backup, t_tool *tool)
{
	char	*dir;

	tool->home = 0;
	tool->flag = 0;
	cmd->cmd++;
	if (!cmd->cmd[0])
	{
		cmd->cmd[0] = get_home_dir(*envh, tool);
		if (!cmd->cmd[0])
			return (1);
		if (tool->home)
			return (0);
	}
	dir = getcwd(0, 0);
	if (dir)
	{
		tool->flag = 1;
		free(*pwd_backup);
		*pwd_backup = dir;
	}
	if (!chdir(cmd->cmd[0]))
		return (cd_helper(envh, cmd->cmd[0], pwd_backup, tool));
	return (cd_complex(envh, cmd, pwd_backup, tool));
}
