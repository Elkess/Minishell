/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_help.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 10:57:48 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/20 11:03:28 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_break_token(int type)
{
	if (type == 1 || type == 2 || type == 3
		|| type == 9 || type == 10)
		return (1);
	return (0);
}

void	update_links(t_token **input, t_token *prev, t_token *target)
{
	if (prev)
		prev->next = target;
	else
		*input = target;
}

void	add_to_index(t_redir *after, int index)
{
	while (after)
	{
		after->index = index +1;
		index++;
		after = after->next;
	}
}