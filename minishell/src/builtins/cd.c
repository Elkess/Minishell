/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 10:01:34 by melkess           #+#    #+#             */
/*   Updated: 2025/06/01 21:31:35 by melkess          ###   ########.fr       */
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

void	cd_helper(t_env **envh, char *cmd, char	**pwd_backup, char *dir)
{
	char	*new_pwd;

	*envh = edit_env("OLDPWD", ft_strdup(*pwd_backup), *envh, 0);
	if (dir)
	{
		// getcwd() succeeded, use the real current directory
		free(*pwd_backup);
		*pwd_backup = getcwd(0, 0);
	}
	else
	{
		if (*pwd_backup && (*pwd_backup)[ft_strlen(*pwd_backup) -1] != '/')
			*pwd_backup = ft_strjoin(*pwd_backup, ft_strjoin("/", cmd, 0), 3); // leaks "/" and old pwd_backups
			else
			*pwd_backup = ft_strjoin(*pwd_backup, cmd, 1); // leaks old pwd_backups
		if (access(*pwd_backup, X_OK))
		ft_putstr_fd("cd: error retrieving current directory: getcwd:"
		" cannot access parent directories: No such file or directory\n", 2);
	}
}

int	cd(t_env **envh, t_tree *cmd, char	**pwd_backup)
{
	/*
	minishell$> unset PWD
	minishell$> unset OLDPWD
	minishell$> echo $PWD
	minishell$> echo $OLDPWD
	minishell$> cd ..
	minishell$> echo $OLDPWD*/
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
	{
		free(*pwd_backup);
		*pwd_backup = dir;
	}

	if (!chdir(cmd->cmd[0]))
		cd_helper(envh, cmd->cmd[0], pwd_backup, dir);
	else
		return (print_err("cd: ", cmd->cmd[0], strerror(errno)), 1);
	*envh = edit_env("PWD", ft_strdup(*pwd_backup), *envh, 0);
	return (0);
}
