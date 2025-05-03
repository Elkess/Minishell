/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lst_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:26:01 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/02 13:49:45 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"

void	init_token(t_token **token, int priority, int type)
{
	(*token)->priority = priority;
	(*token)->type = type;
}

void init_type(t_token **token)
{
    if (!token || !*token || !(*token)->value)
        return ;

    if (!ft_strcmp((*token)->value, "("))
        init_token(token, prio_open_par, TOKEN_PAREN_OPEN);
    else if (!ft_strcmp((*token)->value, ")"))
        init_token(token, prio_close_par, TOKEN_PAREN_CLOSE);
    else if (!ft_strcmp((*token)->value, "&&"))
        init_token(token, prio_and, TOKEN_AND);
    else if (!ft_strcmp((*token)->value, "||"))
        init_token(token, prio_or, TOKEN_OR);
    else if (!ft_strcmp((*token)->value, "|"))
        init_token(token, prio_pipe, TOKEN_PIPE);
    else if (!ft_strcmp((*token)->value, ">"))
        init_token(token, prio_redir_out, TOKEN_REDIR_OUT);
    else if (!ft_strcmp((*token)->value, ">>"))
        init_token(token, prio_appand, TOKEN_REDIR_APPEND);
    else if (!ft_strcmp((*token)->value, "<<"))
        init_token(token, prio_here_doc, TOKEN_REDIR_HEREDOC);
    else if (!ft_strcmp((*token)->value, "<"))
        init_token(token, prio_redir_in, TOKEN_REDIR_IN);
    else
        init_token(token, 0, TOKEN_WORD);
}

void	init_type_utils(t_token *token, t_token *next_token)
{
	if (next_token && token)
	{
		if (next_token->type == TOKEN_REDIR_OUT)
		init_token(&token, 0, TOKEN_FILERED_OUT);
		else if (next_token->type == TOKEN_REDIR_IN)
			init_token(&token, 0, TOKEN_FILERED_IN);
		else if (next_token->type == TOKEN_REDIR_APPEND)
			init_token(&token, 0, TOKEN_FILERED_APPEND);
		else if (next_token->type == TOKEN_REDIR_HEREDOC)
			init_token(&token, 0, TOKEN_FILERED_HEREDOC);
		else
			init_token(&token, 0, TOKEN_WORD);
	}
}