/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lst_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 08:09:06 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/13 11:30:40 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*update_token(t_token **token)
{
	t_token	*current;
	t_token	*prev;
	t_token	*next;

	current = *token;
	prev = NULL;
	next = NULL;
	while (current != NULL)
	{
		next = current->next;
		if (current->type == 4)
		{
			if (prev == NULL)
				*token = next;
			else
				prev->next = next;
			current = next;
		}
		else
		{
			prev = current;
			current = next;
		}
	}
	return (*token);
}

static int	has_concat_quotes(char *value)
{
	int	i;

	i = 0;
	while (value && value[i])
	{
		if ((value[i] == '\'' || value[i] == '"')
			&& (i == 0 || (i > 0 && value[i - 1] != '\\')))
			return (1);
		i++;
	}
	return (0);
}

void	init_quote_token(t_token *token)
{
	t_token	*current;
	char	*val;
	size_t	len;

	current = token;
	while (current)
	{
		if (current->type == 0)
		{
			val = current->value;
			len = ft_strlen(val);
			if (len >= 2 && val[0] == val[len - 1]
				&& (val[0] == '\'' || val[0] == '"'))
			{
				if (val[0] == '\'')
					current->type = 16;
				else
					current->type = 17;
			}
			else if (has_concat_quotes(val))
				current->type = 18;
		}
		current = current->next;
	}
}
