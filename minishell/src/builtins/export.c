/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:34:29 by melkess           #+#    #+#             */
/*   Updated: 2025/06/20 23:25:08 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**sorting_str(char **str)
{
	size_t	i;
	size_t	j;
	char	*tmp;

	i = 0;
	j = 0;
	while (str[i])
	{
		j = 0;
		while (str[j])
		{
			if (ft_strcmp(str[j], str[i]) > 0)
			{
				tmp = str[j];
				str[j] = str[i];
				str[i] = tmp;
			}
			j++;
		}
		i++;
	}
	return (str);
}

char	**filling_str(t_env *envh, t_tool *tool)
{
	size_t	i;
	char	**str;

	str = malloc((ft_envlen(envh) +1) * sizeof(char *));
	if (!str)
		return (NULL);
	add_to_grbg(&tool->grbg, str);
	i = 0;
	while (envh)
	{
		str[i] = ft_strjoin(envh->key, "=", tool);
		str[i] = ft_strjoin(str[i], envh->value, tool);
		i++;
		envh = envh->next;
	}
	str[i] = NULL;
	return (str);
}

void	print_export(t_env *envh, t_tool *tool)
{
	size_t	i;
	char	**str;
	t_env	*holder;

	1 && (i = 0, holder = envh);
	str = sorting_str(filling_str(envh, tool));
	while (str[i])
	{
		while (envh)
		{
			if (envh->key
				&& !ft_strncmp(str[i], envh->key, ft_strlen(envh->key))
				&& str[i][ft_strlen(envh->key)] == '=')
			{
				if (!envh->value)
					printf("declare -x %s\n", envh->key);
				else
					printf("declare -x %s=\"%s\"\n", envh->key, envh->value);
				break ;
			}
			envh = envh->next;
		}
		envh = holder;
		i++;
	}
}

int	ft_export(t_env **envh, t_tree *tree, t_tool *tool)
{
	char	*key;
	char	*val;

	tree->cmd++;
	key = NULL;
	val = NULL;
	if (tree->cmd[0])
		return (manipulate_export(envh, tree, key, val));
	else
		print_export(*envh, tool);
	return (0);
}
