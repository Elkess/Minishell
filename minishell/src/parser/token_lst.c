/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:13:04 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/07 16:49:37 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"

// void	init_token(t_token **token, int priority, int type)
// {
// 	(*token)->priority = priority;
// 	(*token)->type = type;
// }

// void	init_type_utils(t_token *token, t_token *next_token)
// {
// 	if (next_token && token)
// 	{
// 		if (next_token->type == TOKEN_REDIR_OUT)
// 		init_token(&token, 0, TOKEN_FILERED_OUT);
// 		else if (next_token->type == TOKEN_REDIR_IN)
// 			init_token(&token, 0, TOKEN_FILERED_IN);
// 		else if (next_token->type == TOKEN_REDIR_APPEND)
// 			init_token(&token, 0, TOKEN_FILERED_APPEND);
// 		else if (next_token->type == TOKEN_REDIR_HEREDOC)
// 			init_token(&token, 0, TOKEN_FILERED_HEREDOC);
// 		else
// 			init_token(&token, 0, TOKEN_WORD);
// 	}
// }

void hundel_quotes_paren(t_tool *tool, char cmd)
{
	if (cmd == '\'' && tool->quoted == 0)
		tool->quoted = 1;
	else if (cmd == '\'' && tool->quoted == 1)
		tool->quoted = 0;
	else if (cmd == '"' && tool->quoted == 0)
		tool->quoted = 2;
	else if (cmd == '"' && tool->quoted == 2)
		tool->quoted = 0;
	
	// hundel parenthes (only when not in quotes)
	if (tool->quoted == 0)
	{
		if (cmd == '(')
			tool->paren += 1;
		else if (cmd == ')' && (tool->paren > 0))
			tool->paren -= 1;
		else if (cmd == ')' && tool->paren == 0)
			tool->paren = 42;
	}
}

int	is_delimter(char c, char d)
{
	if (c == '|' || c == '&' || c == '>' || c == '<' || c == '(' || c == ')' ||
        c == '\'' || c == '"' || c == '$' || c == '*')
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
        else if (c == '$')
            return (TOKEN_DOLLAR);
        else if (c == '*')
            return (TOKEN_WILDCARD);
	}
	return (0); // if just char
}

int is_space(char c)
{
    if ((c >= 9 && c <= 13) || c == 32)
        return (1);
    return (0);
}


int create_delim_token(char *cmd, int i, t_token **token, t_tool *tool)
{
    int type;
    t_token *new_token;

    type = is_delimter(cmd[i], cmd[i + 1]);

    if (type == TOKEN_SINGL_AND)
    {
        tool->anderr = 1; // Single & is an error
    }

    if (type == TOKEN_AND || type == TOKEN_OR
        || type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HEREDOC)
    {
        new_token = lst_new(ft_my_strdup(&cmd[i], 2, tool), tool);
        new_token->type = type;
        lst_add_back(token, new_token);
        i += 2;
    }
    else
    {
        new_token = lst_new(ft_my_strdup(&cmd[i], 1, tool), tool);
        new_token->type = type;
        lst_add_back(token, new_token);
        i += 1; // Skip one character
    }

    return (i);
}

int  create_space_token(t_token **token, char *cmd, int i, t_tool *tool)
{
	t_token	*new_token;
	char	*space_str;
	int		n;

	new_token = NULL;
	space_str = NULL;
	n = i;
	while (cmd[i] && is_space(cmd[i]))
		i++;
	space_str = ft_my_strdup(&cmd[n], i - n, tool);

	new_token = lst_new(space_str, tool);
    new_token->type = TOKEN_SPACE;
	new_token->priority = 0;
    lst_add_back(token, new_token);

	return (i);
}

int	create_cmd_token(t_token **token, char *cmd, int i, t_tool *tool)
{
	t_token	*new_token;
	char	*word_str;
    int     end;
	
	new_token = NULL;
	word_str = NULL;
    end = i;
    while (cmd[end] && (tool->quoted || (!is_space(cmd[end]) && !is_delimter(cmd[end], cmd[end + 1]))))
        hundel_quotes_paren(tool, cmd[++end]);
    
    if (end != i)
    {
    	word_str = ft_my_strdup(&cmd[i], end - i, tool);
		new_token = lst_new(word_str, tool);
    	new_token->type = TOKEN_WORD;
		new_token->priority = 0;
        lst_add_back(token, new_token);
		
        return (end);
    }
    return (i + 1);
}


t_token	*tokens_lst(char *cmd, t_tool *tool)
{
	t_token	*token;
	t_token	*next_token; 
	int	i;

	i = 0;
	tool->quoted = 0;
	tool->anderr = 0;
	tool->paren = 0;
	token = NULL;
	next_token = lastone(token);
	
	while (cmd && cmd[i])
	{
		// if (cmd[i] == 32 && cmd[i + 1] == '\0')  // for space at end
		// 	return (token);
		hundel_quotes_paren(tool, cmd[i]);
		if (is_space(cmd[i]))
			i = create_space_token(&token, cmd, i, tool);
		else if (cmd[i] && (is_delimter(cmd[i], cmd[i + 1]) > 0) && tool->quoted == 0)
			i = create_delim_token(cmd, i, &token, tool);
		else
			i = create_cmd_token(&token, cmd, i, tool);
	}
	return (token); // just for while 
}
