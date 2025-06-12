/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 14:12:29 by melkess           #+#    #+#             */
/*   Updated: 2025/06/12 15:38:33 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_attributes(t_env *node)
{
	if (node) 
	{
		free(node->key);
		node->key = NULL;
		free(node->value);
		node->value = NULL;
		node->next = NULL;
		// free(node); // TODO: be careful 
		node = NULL;
	}
}

t_env	*delete_var(t_env *envh, char *key)
{
	t_env	*head;

	head = envh;
	while (head)
	{
		if (!ft_strcmp(head->key, key))
		{
			if (head && head->prev)
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
				// puts("UP");
			if (search_for_defaults(*envh, args[i]))
				*envh = delete_var(*envh, args[i]);
				// puts("DOWNY");
		}
		else
		{
			print_err("unset: `", args[i], "': not a valid identifier");
			n = 1;
		}
		i++;
	}
	
	return (n);
}