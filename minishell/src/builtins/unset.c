/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 14:12:29 by melkess           #+#    #+#             */
/*   Updated: 2025/05/14 08:55:06 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor.h"

void	free_attributes(t_env *node)
{
	free(node->key);
	node->key = NULL;
	free(node->value);
	node->value = NULL;
	node->next = NULL;
	node->value = NULL;
	free(node);
}

t_env	*delete_var(t_env *envh, char *key)
{
	t_env	*head;

	head = envh;
	while (head)
	{
		if (!ft_strcmp(head->key, key))
		{
			if (head->prev)
			{
				head->prev->next = head->next;
				free_attributes(head);	
			}
			else
			{
				envh = head->next;
				free_attributes(head);
			}
			break;
		}
		head = head->next;
	}
	return (envh);
}

int	unset(t_env **envh, char **args)
{
	size_t	i;
	char	*key;
	char	*val;
	int		n;

	i = 0;
	n = 0;
	while (args[i])
	{
		if (is_valid_key(args[i]))
		{
			if (search_for_defaults(*envh, args[i]))
				*envh = delete_var(*envh, args[i]);
		}
		else
		{
			ft_putstr_fd(ft_strjoin("minishell: unset: ", ft_strjoin(args[i], "': not a valid identifier\n")), 2);
			n = 1;
		}
		i++;
	}
	return (n);
}