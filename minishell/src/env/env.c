/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:08:39 by melkess           #+#    #+#             */
/*   Updated: 2025/06/19 15:56:09 by melkess          ###   ########.fr       */
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

char	*extract_data(char *line, int flag)
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

int	handle_defaults(t_env **envh)
{
	char	*vals[4];
	char	*keys[4];
	size_t	len;

	len = 0;
	while (len < 4)
	{
		keys[len] = NULL;
		vals[len] = NULL;
		len++;
	}
	len = 0;
	if (!search_for_defaults(*envh, "OLDPWD"))
		keys[0] = ft_strdup_env("OLDPWD", 0);
	if (!search_for_defaults(*envh, "PWD"))
	{
		keys[1] = ft_strdup_env("PWD", 0);
		vals[1] = getcwd(NULL, 0);
		if (!vals[1])
		{
			printf("bash: cd: getcwd Failed\n");
			return (1);
		}
	}
	if (!search_for_defaults(*envh, "SHLVL"))
	{
		keys[2] = ft_strdup_env("SHLVL", 0);
		vals[2] = ft_strdup_env("1", 0);
	}
	if (!search_for_defaults(*envh, "PATH"))
	{
		keys[3] = ft_strdup_env("PATH", 0);
		vals[3] = ft_strdup_env("/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.", 0);
	}
	while (len < 4)
	{
		if(!search_for_defaults(*envh, keys[len]))
			*envh = append_node(*envh, keys[len], vals[len]);
		len++;
	}
	return (0);
}
long long	ft_atoll(const char *str)
{
	long long	result;
	int			sign;

	if (!str || !*str)
		return (1);
	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;

	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}

	return (result * sign);
}
char	*handle_shlvl(char *val)
{	// TODO: Test it more; you are NOT fully understand how it works 
	long long		raw_input;
	unsigned int	wrapped;
	int				parsed;

	raw_input = ft_atoll(val);
	free(val);
	if (raw_input == 999)
		return (ft_strdup_env("", 0));
	if (raw_input < INT_MIN || raw_input > INT_MAX)
	{
		wrapped = (unsigned int)raw_input;
		parsed = wrapped +1;
	}
	else
		parsed = (int)raw_input +1;
	if (parsed < 0)
		parsed = 0;
	else if (parsed > 1000)
		parsed = 1;
	val = ft_itoa_env(parsed);
	return (ft_strdup_env(val, 1));
}

t_env	*fill_env(char **envp)
{
	size_t	i;
	t_env	*envh;
	char	*key;
	char	*val;

	envh = NULL;
	i = 0;
	if (envp && *envp)
	{
		while (envp[i])
		{
			val = extract_data(envp[i], 1);
			key = extract_data(envp[i], 0);
			if (!ft_strcmp(key, "SHLVL"))
				val = handle_shlvl(val);
			if (!ft_strcmp(key, "OLDPWD"))
			{
				char *tmp = val;
				
				val = NULL;
				free(tmp);
			}
			envh = append_node(envh, key, val);
			i++;
		}
	}
	if (handle_defaults(&envh))
		return (NULL);
	return (envh);
}
