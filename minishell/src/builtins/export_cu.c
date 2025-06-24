/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 23:22:47 by melkess           #+#    #+#             */
/*   Updated: 2025/06/23 17:00:14 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_plus_case(t_env **envh, char *key, char *val, char *arg)
{
	char	*tmp;

	tmp = key;
	key = ft_substr_env(key, 0, ft_strlen(key) - 1);
	free(tmp);
	if (is_valid_key(key))
		*envh = edit_env(key, val, *envh, 1);
	else
	{
		print_err("export: `", arg, "': not a valid identifier");
		free(key);
		free(val);
		return (1);
	}
	return (0);
}

static int	handle_normal_case(t_env **envh, char *key, char *val, char *arg)
{
	if (key && *key && is_valid_key(key))
		*envh = edit_env(key, val, *envh, 0);
	else
	{
		print_err("export: `", arg, "': not a valid identifier");
		free(key);
		free(val);
		return (1);
	}
	return (0);
}

int	manipulate_export(t_env **envh, t_tree *cmd, char *key, char *val)
{
	size_t	i;
	int		status;

	status = 0;
	i = 0;
	while (cmd->cmd[i])
	{
		key = ext_data(cmd->cmd[i], 0);
		val = ext_data(cmd->cmd[i], 1);
		if (val && key && *key && *key != '+' && key[ft_strlen(key) - 1] == '+')
		{
			if (handle_plus_case(envh, key, val, cmd->cmd[i]))
				status = 1;
		}
		else
		{
			if (handle_normal_case(envh, key, val, cmd->cmd[i]))
				status = 1;
		}
		i++;
	}
	return (status);
}
