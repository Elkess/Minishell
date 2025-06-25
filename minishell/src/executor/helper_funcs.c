/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_funcs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:39:01 by melkess           #+#    #+#             */
/*   Updated: 2025/06/25 11:28:54 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_err(char *msg1, char *arg, char *msg2)
{
	int	fd;

	fd = dup(1);
	if (fd < 0)
		(perror("dup failed"));
	if (dup2(2, 1) < 0)
		(perror("dup failed"));
	if (!msg1)
		printf("minishell: %s: %s\n", arg, msg2);
	else
		printf("minishell: %s%s %s\n", msg1, arg, msg2);
	if (dup2(fd, 1) < 0)
		(perror("dup failed"));
}

int	is_valid_key(char *key)
{
	size_t	i;

	if (!key || !*key)
		return (1);
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

void	print_rest(char **s, int n, t_tool *tool)
{
	size_t	i;
	char	*buffer;

	buffer = NULL;
	i = 0;
	while (s[i])
	{
		buffer = ft_strjoin(buffer, s[i++], tool);
		if (s[i])
			buffer = ft_strjoin(buffer, " ", tool);
	}
	if (!n)
		buffer = ft_strjoin(buffer, "\n", tool);
	write(1, buffer, ft_strlen(buffer));
}

char	**struct_to_darr(t_env *envh, t_tool *tool)
{
	size_t	len;
	char	**env;

	len = ft_envlen(envh);
	env = malloc((len +1) * sizeof(char *));
	if (!env)
		(NULL);
	add_to_grbg(&tool->grbg, env);
	len = 0;
	while (envh)
	{
		if (envh->value)
		{
			env[len] = ft_strjoin(envh->key, "=", tool);
			env[len] = ft_strjoin(env[len], envh->value, tool);
			len++;
		}
		envh = envh->next;
	}
	env[len] = NULL;
	return (env);
}

void	ft_dup(int *io, int flag, t_tool *tool)
{
	if (flag)
	{
		io[0] = dup(STDIN_FILENO);
		io[1] = dup(STDOUT_FILENO);
		if (io[0] == -1 || io[1] == -1)
		{
			(close(io[0]), close(io[1]));
			clear_garbcoll(tool->grbg);
			(perror("dup failed"), exit (1));
		}
	}
	else
	{
		if (dup2(io[0], 0) < 0 || dup2(io[1], 1) < 0)
		{
			(close(io[0]), close(io[1]));
			clear_garbcoll(tool->grbg);
			(perror("dup failed"), exit (1));
		}
		(close(io[0]), close(io[1]));
	}
}
