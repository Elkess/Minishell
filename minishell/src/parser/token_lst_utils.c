/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lst_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:26:01 by sgmih             #+#    #+#             */
/*   Updated: 2025/04/24 10:31:53 by sgmih            ###   ########.fr       */
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

void	init_type(t_token **token)
{
	if (!ft_strcmp((*token)->value, "("))
		(*token)->type = TOKEN_PAREN_OPEN;
	else if (!ft_strcmp((*token)->value, ")"))
		(*token)->type = TOKEN_PAREN_CLOSE;
	else if (!ft_strcmp((*token)->value, "&&"))
		(*token)->type = TOKEN_AND;
	else if (!ft_strcmp((*token)->value, "||"))
		(*token)->type = TOKEN_OR;
	else if (!ft_strcmp((*token)->value, "|"))
		(*token)->type = TOKEN_PIPE;
	else if (!ft_strcmp((*token)->value, ">"))
		(*token)->type = TOKEN_REDIR_OUT;
	else if (!ft_strcmp((*token)->value, ">>"))
		(*token)->type = TOKEN_REDIR_APPEND;
	else if (!ft_strcmp((*token)->value, "<<"))
		(*token)->type = TOKEN_REDIR_HEREDOC;
	else if (!ft_strcmp((*token)->value, "<"))
		(*token)->type = TOKEN_REDIR_IN;
	else
		(*token)->type = 0; // unknow type
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
			token->type = TOKEN_FILERED_OUT;
		else if (next_token->type == TOKEN_REDIR_IN)
			token->type = TOKEN_FILERED_IN;
		else if (next_token->type == TOKEN_REDIR_APPEND)
			token->type = TOKEN_FILERED_APPEND;
		else if (next_token->type == TOKEN_REDIR_HEREDOC)
			token->type = TOKEN_FILERED_HEREDOC;
		else
			token->type = TOKEN_WORD;
	}
}