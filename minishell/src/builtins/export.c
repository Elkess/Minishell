/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:34:29 by melkess           #+#    #+#             */
/*   Updated: 2025/06/19 15:48:37 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//TODO: Export need more search and testing Cases

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

size_t	ft_envlen(t_env *envh)
{
	size_t	i;

	i = 0;
	while (envh)
	{
		i++;
		envh = envh->next;
	}
	return (i);
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

	i = 0;
	holder = envh;
	str = sorting_str(filling_str(envh, tool));
	while (str[i])
	{
		while (envh)
		{
			if (envh->key && !ft_strncmp(str[i], envh->key, ft_strlen(envh->key)) && str[i][ft_strlen(envh->key)] == '=')
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

int	manipulate_export(t_env **envh, t_tree *cmd1, char *key, char *val)
{
	size_t	i;
	char	*tmp;
	i = 0;
	while (cmd1->cmd[i])
	{
		key = extract_data(cmd1->cmd[i], 0); //leaks
		val = extract_data(cmd1->cmd[i], 1); //leaks
		if (key && *key  && *key != '+' && key[ft_strlen(key) -1] == '+' )
		{
			tmp = key;
			key = ft_substr_env(key, 0, ft_strlen(key) -1);//free old key
			free(tmp);
			if (is_valid_key(key))
				*envh = edit_env(key, val, *envh, 1);
			else
				(print_err("export: `", cmd1->cmd[i],
					"': not a valid identifier"), free(key)); // free attrs
		}
		else if (key && *key && is_valid_key(key))
			*envh = edit_env(key, val, *envh, 0);
		else
			(print_err("export: `", cmd1->cmd[i],
				"': not a valid identifier"), free(key));
		i++;
	}
	return (0);
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
