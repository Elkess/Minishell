/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:33:52 by melkess           #+#    #+#             */
/*   Updated: 2025/05/18 08:37:26 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	env(t_env *envh)
{
	char	*env_join;
	char	**env;
	size_t	i;

	if (!search_for_defaults(envh, "PATH"))
	{
		printf("minishell: env: 4No such file or directory\n");
		return (127);	
	}
	i = 0;
	env_join = NULL;
	env = struct_to_darr(envh);
	while (env && env[i])
	{
		env_join = ft_strjoin(env_join, env[i]); // leaks
		env_join = ft_strjoin(env_join, "\n");
		i++;
	}
	ft_putstr_fd(env_join, 1);
	return (0);
}
