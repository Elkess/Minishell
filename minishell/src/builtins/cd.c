/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 10:01:34 by melkess           #+#    #+#             */
/*   Updated: 2025/04/20 09:14:42 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor.h"

void	cd(t_env **envh, t_tree *cmd)
{
	DIR		*dir;
	char	*_dir;
	t_env	*var;

	cmd->cmd++;
	if (!cmd->cmd[0])
	{
		var = search_for_defaults(*envh, "HOME");
		if (!var)
		{
			printf("minishell: HOME not set\n");
			exit (11);
		}
		cmd->cmd[0] = var->value;
	}
	_dir = getcwd(NULL, 0);
	dir = opendir(cmd->cmd[0]);
	if (!dir)
	{
		printf("bash: cd: %s : No such file or directory\n", cmd->cmd[0]);
		return ;
	}
	if (chdir(cmd->cmd[0]) == -1)
		exit (91);
	*envh = edit_env("OLDPWD", _dir, *envh, 0);
	// free(_dir); //??
	_dir = getcwd(NULL, 0);
	*envh = edit_env("PWD", _dir, *envh, 0);
	// free(_dir);
	closedir(dir); // ??
}
