/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lst_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:26:01 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/10 09:23:06 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"

static int	has_concat_quotes(char *value)
{
	int		i;
	int		found_quote;
	char	quote_type;

	i = 0;
	found_quote = 0;
	while (value && value[i])
	{
		if ((value[i] == '\'' || value[i] == '"') && 
			(i == 0 || (i > 0 && value[i - 1] != '\\')))
		{
			quote_type = value[i];
			i++;
			while (value[i] && value[i] != quote_type)
				i++;
			if (value[i] == quote_type && value[i + 1] != '\0')
				return (1);
		}
		if (value[i])
			i++;
	}
	return (0);
}

static char	*extract_quoted_part(char *str, int *pos, t_tool *tool)
{
	int		start;
	char	quote_type;
	int		i;

	i = *pos;
	quote_type = str[i];
	start = i;
	i++;
	while (str[i] && str[i] != quote_type)
		i++;
	if (str[i] == quote_type)
		i++;
	*pos = i;
	return (ft_my_strdup(&str[start], i - start, tool));
}

static void	split_concat_tokens(t_token **head, t_token *token, t_tool *tool)
{
	t_token	*prev;
	t_token	*current;
	t_token	*next;
	char	*value;
	int		pos;
	char	*part;

	prev = NULL;
	current = *head;
	while (current != token)
	{
		prev = current;
		current = current->next;
	}
	next = token->next;
	value = token->value;
	pos = 0;
	
	part = extract_quoted_part(value, &pos, tool);
	if (prev)
	{
		prev->next = lst_new(part, tool);
		current = prev->next;
	}
	else
	{
		*head = lst_new(part, tool);
		current = *head;
	}
	current->type = TOKEN_WORD;
	current->priority = token->priority;

	while (value[pos])
	{
		part = extract_quoted_part(value, &pos, tool);
		current->next = lst_new(part, tool);
		current = current->next;
		current->type = TOKEN_WORD;
		current->priority = token->priority;
	}
	
	current->next = next;
	
	free(token->value);
	free(token);
}

static void	init_quote_token(t_token *token)
{
    t_token *current = token;
    while (current)
    {
        if (current->type == TOKEN_WORD)
        {
            char *val = current->value;
            size_t len = strlen(val);
            if (len >= 2 && val[0] == val[len - 1] && (val[0] == '\'' || val[0] == '"'))
            {
                if (val[0] == '\'')
                    current->type = TOKEN_SINGLE_QUOTED_WORD;
                else
                    current->type = TOKEN_DOUBLE_QUOTED_WORD;
            }
        }
        current = current->next;
    }
}

t_token	*check_token(t_token **token, t_tool *tool)
{
	t_token	*current;
	t_token	*next;

	if (!token || !*token)
		return (NULL);
	current = *token;
	while (current)
	{
		next = current->next;
		if (current->type == TOKEN_WORD && has_concat_quotes(current->value))
			split_concat_tokens(token, current, tool);
		current = next;
	}
	init_quote_token(*token);
	// if (pars_err(token, tool))
	// 	return (NULL);
	//update_lst_redir for fill s_redir 
	// pars_trime(*token, tool);
	return (*token);
}
