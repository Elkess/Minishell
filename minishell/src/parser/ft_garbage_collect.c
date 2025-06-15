/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_garbage_collect.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:29:32 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/15 10:43:17 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_garbcoll	*new_garbcoll(void *value)
{
	t_garbcoll	*token;

	token = (t_garbcoll *)malloc(sizeof(t_garbcoll));
	if (!token)
		return (NULL);
	token->ptr = value;
	token->next = NULL;
	return (token);
}

void	add_to_grbg(t_garbcoll **head, void *value)
{
	t_garbcoll	*tmp;
	t_garbcoll	*token;

	token = new_garbcoll(value);
	if (!head || !token)
	{
		if (!token)
			write(2, "minishell$> : malloc error\n", 27);
		return ;
	}
	if (!*head)
	{
		*head = token;
		return ;
	}
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = token;
		return ;
	}
}

// check if duble free issu
void	clear_garbcoll(t_garbcoll *head)
{
	t_garbcoll	*token;

	if (!head)
		return ;
	token = head;
	while (head)
	{
		free(head->ptr);
		token = head;
		head = head->next;
		free(token);
		// if (token)
		// 	free(token->ptr);
		// free(token);
		// token = head;
	}
}
