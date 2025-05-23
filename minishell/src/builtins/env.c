/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:33:52 by melkess           #+#    #+#             */
/*   Updated: 2025/05/21 18:58:12 by melkess          ###   ########.fr       */
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
		printf("minishell: env: No such file or directory\n");
		return (127);	
	}
	i = 0;
	env_join = NULL;
	env = struct_to_darr(envh);
	while (env && env[i])
	{
		env_join = ft_strjoin(env_join, env[i], 1); // leaks in env_join use tmp or whatever
		env_join = ft_strjoin(env_join, "\n", 1);
		i++;
	}
	ft_putstr_fd(env_join, 1);
	free(env_join);
	free_twod(env);
	return (0);
}
