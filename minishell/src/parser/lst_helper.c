/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:23:41 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/10 09:22:40 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"

static void init_type(t_token **token)
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

t_token	*lst_new(void *str, t_tool *tool)
{
	t_token	*token;

	if (!str)
		return (NULL);
	token = (t_token *) malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	add_to_grbg(&tool->grbg, token);
	token->value = str;
	init_type(&token);
	token->next = NULL;
	return (token);
}

t_token	*lastone(t_token *head)
{
	if (!head)
		return (NULL);
	while (head->next)
		head = head->next;
	return (head);
}

void	lst_add_back(t_token **head, t_token *token)
{
	t_token	*tmp;

	if (!head || !token)
	{
		if (!token)
			write(2, "minishell$> : malloc error\n", 27);
		return ;
	}
	if (!*head)
	{
		*head = token;
		(*head)->next = NULL;
		return ;
	}
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = token;
		token->next = NULL;
		return ;
	}
}
