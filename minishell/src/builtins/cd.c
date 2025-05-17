/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 10:01:34 by melkess           #+#    #+#             */
/*   Updated: 2025/05/14 15:30:14 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor.h"

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

void	cd_helper(t_env **envh, char *cmd, char	**pwd_backup, char *dir)
{
	*envh = edit_env("OLDPWD", ft_strdup(*pwd_backup), *envh, 0);
	if (dir)
		*pwd_backup = getcwd(0, 0);
	else
	{
		if (*pwd_backup && (*pwd_backup)[ft_strlen(*pwd_backup) -1] != '/')
			*pwd_backup = ft_strjoin(*pwd_backup, ft_strjoin("/", cmd));
		else
			*pwd_backup = ft_strjoin(*pwd_backup, cmd);
		ft_putstr_fd("cd: error retrieving current directory: getcwd:"
		" cannot access parent directories: No such file or directory\n", 2);
	}
}

int	cd(t_env **envh, t_tree *cmd, char	**pwd_backup)
{
	char	*dir;

	cmd->cmd++;
	if (!cmd->cmd[0])
	{
		cmd->cmd[0] = get_home_dir(*envh);
		if (!cmd->cmd[0])
			return (1);
	}
	dir = getcwd(0, 0);
	if (dir)
		*pwd_backup = dir;
	if (!chdir(cmd->cmd[0]))
		cd_helper(envh, cmd->cmd[0], pwd_backup, dir);
	else
	{
		ft_putstr_fd(ft_strjoin("minishell: cd: ",
			ft_strjoin(cmd->cmd[0], ": No such file or directory\n")), 2);
		return (1);
	}
	*envh = edit_env("PWD", *pwd_backup, *envh, 0);
	return (0);
}
