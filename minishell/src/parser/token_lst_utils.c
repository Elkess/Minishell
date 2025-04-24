/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lst_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:26:01 by sgmih             #+#    #+#             */
/*   Updated: 2025/04/24 12:03:09 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"

/******************************************** function libft **********************************************/

static int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (s2[i] - s1[i]);
		i++;
	}
	return (0);
}
/******************************************** function libft **********************************************/

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

char	*ft_my_strdup(const char *s1, size_t size, t_tool *tool)
{
	char		*dup;
	size_t		i;

	i = 0;
	dup = (char *) malloc(size + 1 * sizeof(char));
	if (!dup)
		return (NULL);
	add_to_grbg(&tool->grbg, dup);
	while (i < size)
	{
		dup[i] = s1[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
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