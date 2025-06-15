/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:33:52 by melkess           #+#    #+#             */
/*   Updated: 2025/06/15 10:25:09 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	env(t_env *envh, t_tool *tool)
{
	char	*env_join;
	char	**env;
	size_t	i;

	i = 0;
	env_join = NULL;
	env = struct_to_darr(envh, tool);
	while (env && env[i])
	{
		env_join = ft_strjoin(env_join, env[i], tool);
		env_join = ft_strjoin(env_join, "\n", tool);
		i++;
	}
	ft_putstr_fd(env_join, 1);
	// free(env_join);
	// free_twod(env);
	return (0);
}
