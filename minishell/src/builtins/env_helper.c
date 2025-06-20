/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 23:42:03 by melkess           #+#    #+#             */
/*   Updated: 2025/06/20 23:42:20 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*edit_env(char *key, char *val, t_env *envh, int should_concat)
{
	t_env	*tmp;

	tmp = envh;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
		{
			free(key);
			if (should_concat)
				tmp->value = ft_strjoin_env(tmp->value, val, 3);
			else
			{
				if (val)
				{
					free(tmp->value);
					tmp->value = val;
				}
			}
			return (envh);
		}
		tmp = tmp->next;
	}
	return (append_node(envh, key, val));
}

char	*ext_data(char *line, int flag)
{
	size_t	i;

	i = 0;
	while (line[i] && line[i] != '=')
		i++;
	if (flag == 1)
	{
		if (!line[i])
			return (NULL);
		return (ft_substr_env(line, ++i, ft_strlen(line)));
	}
	return (ft_substr_env(line, 0, i));
}
