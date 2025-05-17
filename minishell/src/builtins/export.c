/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:34:29 by melkess           #+#    #+#             */
/*   Updated: 2025/05/14 08:54:03 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor.h"
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

char	**filling_str(t_env *envh)
{
	size_t	i;
	char	**str;

	str = malloc((ft_envlen(envh) +1) * sizeof(char *)); // leaks
	i = 0;
	while (envh)
	{
		str[i] = ft_strjoin((envh)->key, "="); // leaks
		str[i] = ft_strjoin(str[i], (envh)->value); // leaks
		i++;
		envh = (envh)->next;
	}
	str[i] = NULL;
	return (str);
}

void	print_export(t_env *envh)
{
	size_t	i;
	char	**str;
	char	*buffer;
	t_env	*holder;

	i = 0;
	buffer = NULL;
	holder = envh; //TODO: NO need for buffering 
	str = sorting_str(filling_str(envh)); //leaks
	while (str[i])
	{
		while (envh)
		{
			if (!ft_strncmp(str[i], envh->key, ft_strlen(envh->key))
				&& str[i][ft_strlen(envh->key)] == '=')
			{
				if (!envh->value)
				{
					// buffer = ft_strjoin(buffer, ft_strjoin("declare -x ", ft_strjoin(envh->key, "\n")));
					printf("declare -x %s\n", envh->key);
				}
				else
				{
					// buffer = ft_strjoin(buffer, ft_strjoin("declare -x ",
					// 	ft_strjoin(envh->key, ft_strjoin("=\"", ft_strjoin(envh->value, "\"\n")))));
					printf("declare -x %s=\"%s\"\n", envh->key, envh->value);
				}
				break ;
			}
			envh = envh->next;
		}
		envh = holder;
		i++;
	}
	// ft_putstr_fd(buffer, 1);
}

int	manipulate_export(t_env **envh, t_tree *cmd1, char *key, char *val)
{
	size_t	i;

	i = 0;
	while (cmd1->cmd[i])
	{
		key = extract_data(cmd1->cmd[i], 0); //leaks
		val = extract_data(cmd1->cmd[i], 1); //leaks
		if (key[ft_strlen(key) -1] == '+' )
		{
			key = ft_substr(key, 0, ft_strlen(key) -1);
			if (is_valid_key(key))
				*envh = edit_env(key, val, *envh, 1);
			else
			{
				ft_putstr_fd(ft_strjoin("minishell10: export: `", ft_strjoin(cmd1->cmd[i], "': not a valid identifier\n")), 2);
				return (1);
			}
		}
		else if (is_valid_key(key))
			*envh = edit_env(key, val, *envh, 0);
		else
		{
			ft_putstr_fd(ft_strjoin("minishell11: export: `", ft_strjoin(cmd1->cmd[i], "': not a valid identifier\n")), 2);
			return (1);
		}
		i++;
	}
	return (0);
}

int	ft_export(t_env **envh, t_tree *tree)
{
	char	*key;
	char	*val;

	tree->cmd++;
	key = NULL;
	val = NULL;
	if (tree->cmd[0])
		return (manipulate_export(envh, tree, key, val));
	else
		print_export(*envh);
	return (0);
}
