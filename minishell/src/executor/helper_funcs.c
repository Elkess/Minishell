/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_funcs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:39:01 by melkess           #+#    #+#             */
/*   Updated: 2025/05/14 14:27:44 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor.h"

t_env	*edit_env(char *key, char *val, t_env *envh, int should_concat)
{
	t_env	*newnode;
	t_env	*tmp;
	int		exist;

	exist = 0;
	tmp = envh; //leaks
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
		{
			if (should_concat)
				tmp->value = ft_strjoin(tmp->value, val);
			else
			{
				if (val)
				{
					free(tmp->value);
					tmp->value = val; //leaks
				}
			}
			return (envh);
		}
		tmp = tmp->next;
	}
	return (append_node(envh, key, val));
}

int	is_valid_key(char *key)
{
	size_t	i;

	i = 0;
	if (!ft_isalpha(key[i]) && key[i] != '_')
		return (0);
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	print_rest(char **s, int n)
{
	size_t	i;
	char	*buffer;

	buffer = NULL;
	i = 0;
	while (s[i])
	{
		buffer= ft_strjoin(buffer, s[i++]);
		if (s[i])
			buffer= ft_strjoin(buffer, " "); //TODO: handle tabs
	}
	if (!n)
		buffer= ft_strjoin(buffer, "\n");
	write(1, buffer, ft_strlen(buffer));
}

