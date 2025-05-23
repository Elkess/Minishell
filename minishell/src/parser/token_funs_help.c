/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_funs_help.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 08:57:56 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/23 07:58:21 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_space(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

void	hundel_quotes_paren(t_tool *tool, char cmd)
{
	if (cmd == '\'' && tool->quoted == 0)
		tool->quoted = 1;
	else if (cmd == '\'' && tool->quoted == 1)
		tool->quoted = 0;
	else if (cmd == '"' && tool->quoted == 0)
		tool->quoted = 2;
	else if (cmd == '"' && tool->quoted == 2)
		tool->quoted = 0;
	if (tool->quoted == 0)
	{
		if (cmd == '(')
			tool->paren += 1;
		else if (cmd == ')' && (tool->paren > 0))
			tool->paren -= 1;
		else if (cmd == ')' && tool->paren == 0)
			tool->paren = -1;
	}
}

int	is_delimter(char c, char d)
{
	if (c == '|' || c == '&' || c == '>' || c == '<' || c == '(' || c == ')')
	{
		if (c == '>')
		{
			if (d == '>')
				return (TOKEN_REDIR_APPEND);
			else
				return (TOKEN_REDIR_OUT);
		}
		else if (c == '<')
		{
			if (d == '<')
				return (TOKEN_REDIR_HEREDOC);
			else
				return (TOKEN_REDIR_IN);
		}
		else if (c == '|')
		{
			if (d == '|')
				return (TOKEN_OR);
			else
				return (TOKEN_PIPE);
		}
		else if (c == '&')
		{
			if (d == '&')
				return (TOKEN_AND);
			else
				return (TOKEN_SINGL_AND);
		}
		else if (c == '(')
			return (TOKEN_PAREN_OPEN);
		else if (c == ')')
			return (TOKEN_PAREN_CLOSE);
	}
	return (0);
}

void	init_token(t_token **token, int priority, int type)
{
	(*token)->priority = priority;
	(*token)->type = type;
}

void	init_redir_file_tokens(t_token *token)
{
	t_token	*current;

	current = token;
	while (current && current->next)
	{
		if (current->type == 6 || current->type == 5
			|| current->type == 7 || current->type == 8)
		{
			if (current->next && current->next->type == 0)
			{
				if (current->type == 6)
					init_token(&current->next, 0, -2);
				else if (current->type == 5)
					init_token(&current->next, 0, -3);
				else if (current->type == 7)
					init_token(&current->next, 0, -5);
				else if (current->type == 8)
					init_token(&current->next, 0, -4);
			}
		}
		current = current->next;
	}
}
