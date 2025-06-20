/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:08:39 by melkess           #+#    #+#             */
/*   Updated: 2025/06/20 23:45:08 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	fill_missing_vars(t_env **envh, char *keys[4], char *va[4])
{
	if (!sh_env(*envh, "OLDPWD"))
		keys[0] = ft_strdup_env("OLDPWD", 0);
	if (!sh_env(*envh, "SHLVL"))
	{
		keys[2] = ft_strdup_env("SHLVL", 0);
		va[2] = ft_strdup_env("1", 0);
	}
	if (!sh_env(*envh, "PATH"))
	{
		keys[3] = ft_strdup_env("PATH", 0);
		va[3] = ft_strdup_env("/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.", 0);
	}
}

static void	append_defaults(t_env **envh, char *keys[4], char *va[4])
{
	size_t	i;

	i = 0;
	while (i < 4)
	{
		if (keys[i] && !sh_env(*envh, keys[i]))
			*envh = append_node(*envh, keys[i], va[i]);
		i++;
	}
}

int	handle_defaults(t_env **envh)
{
	char	*va[4];
	char	*keys[4];
	size_t	i;

	i = 0;
	while (i < 4)
	{
		keys[i] = NULL;
		va[i] = NULL;
		i++;
	}
	if (!sh_env(*envh, "PWD"))
	{
		keys[1] = ft_strdup_env("PWD", 0);
		va[1] = getcwd(NULL, 0);
		if (!va[1])
		{
			printf("bash: cd: getcwd Failed\n");
			return (1);
		}
	}
	fill_missing_vars(envh, keys, va);
	append_defaults(envh, keys, va);
	return (0);
}

// TODO: Test it more; you are NOT fully understand how it works 

char	*handle_shlvl(char *val)
{
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
	char	*tmp;

	1 && (envh = NULL, i = 0);
	if (envp && *envp)
	{
		while (envp[i])
		{
			1 && (val = ext_data(envp[i], 1), key = ext_data(envp[i], 0));
			if (!ft_strcmp(key, "SHLVL"))
				val = handle_shlvl(val);
			if (!ft_strcmp(key, "OLDPWD"))
			{
				1 && (tmp = val, val = NULL);
				free(tmp);
			}
			1 && (envh = append_node(envh, key, val), i++);
		}
	}
	if (handle_defaults(&envh))
		return (NULL);
	return (envh);
}
