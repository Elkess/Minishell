/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 07:50:25 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/17 18:14:15 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*append_node(t_env *head, char *key, char *val)
{
	t_env	*tmp;
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		(free_envh(head), exit(1));
	node->key = key;
	node->value = val;
	node->next = NULL;
	if (!head)
		return (node);
	tmp = head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
	return (head);
}

t_env	*search_for_defaults(t_env *envh, char *key)
{
	if (!key)
		return (NULL);
	while (envh)
	{
		if (!ft_strcmp(envh->key, key))
			return (envh);
		envh = envh->next;
	}
	return (NULL);
}

void	free_envh(t_env *envh)
{
	t_env	*tmp;

	while (envh)
	{
		free(envh->key);
		free(envh->value);
		tmp = envh;
		envh = envh->next;
		free(tmp);
	}
}
