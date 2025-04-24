/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:13:04 by sgmih             #+#    #+#             */
/*   Updated: 2025/04/24 10:22:34 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"

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
	
	// hundel parenthes 
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
			return (TOKEN_SINGL_AND);
		}
		else if (c == '(')
			return (TOKEN_PAREN_OPEN);
		else if (c == ')')
			return (TOKEN_PAREN_CLOSE);
	}
	return (0); // if just char
}

void	create_delim_token(char *cmd, int *i, t_token **token, t_tool *tool)
{
	int type;

	type = is_delimter(cmd[*i], cmd[*i + 1]);
	if (type == TOKEN_AND || type == TOKEN_OR
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HEREDOC)
	{
		lst_add_back(token, lst_new(ft_my_strdup(&cmd[*i], 2, tool), tool));
		*i += 1;
	}
	else
	{
		lst_add_back(token, lst_new(ft_my_strdup(&cmd[*i], 1, tool), tool));
	}
}

void	create_tokens(t_token **token, char *cmd, int *i, t_tool *tool)
{
	int	end;
	t_token	*next_token;

	next_token = lastone(*token);
	if (next_token && cmd[*i] && (next_token->type == 5 || next_token->type == 4 || next_token->type == 6 || next_token->type == 7))
	{
		end = *i;
		while (cmd[end] && (tool->quoted || (cmd[end] != ' ' && cmd[end] != '\t' && !is_delimter(cmd[end], cmd[end + 1]))))
			hundel_quotes_paren(tool, cmd[++end]);
	}
	else
	{
		end = *i;
		while (cmd[end] && (tool->quoted || !is_delimter(cmd[end], cmd[end + 1])))
			hundel_quotes_paren(tool, cmd[++end]);
	}
	if (end != *i)
	{
		lst_add_back(token, lst_new(ft_my_strdup(&cmd[*i], end - *i, tool), tool));
		*i = end - 1;
		init_type_utils(lastone(*token), next_token);
	}
}

t_token	*tokens_lst(char *cmd, t_tool *tool)
{
	t_token	*token;
	int	i;

	i = 0;
	tool->quoted = 0;
	tool->anderr = 0;
	tool->paren = 0;
	token = NULL;
	while (cmd && cmd[i])
	{
		while (cmd[i] && (cmd[i] == ' ' || cmd[i] == '\t'))
			i++;
		if (!cmd[i])
			break ;
		hundel_quotes_paren(tool, cmd[i]);
		if (cmd[i] && (is_delimter(cmd[i], cmd[i + 1]) > 0) && tool->quoted == 0)
		{
			if (is_delimter(cmd[i], cmd[i + 1]) == 10)
				tool->anderr = 1; // for test in after 
			else
				create_delim_token(cmd, &i, &token, tool);
		}
		else
			create_tokens(&token, cmd, &i, tool);
		i++;
	}
	return (check_token(&token, tool)); // just for while 
}


// void create_delim_token(char *cmd, int *i, t_token **token, t_tool *tool)
// {
//     int type;
//     type = is_delimter(cmd[*i], cmd[*i + 1]);
//     if (type == TOKEN_AND || type == TOKEN_OR ||
//         type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HEREDOC)
//     {
//         t_token *new_token = lst_new(ft_my_strdup(&cmd[*i], 2, tool), tool);
//         if (new_token)
//             new_token->type = type; // Set token type
//         lst_add_back(token, new_token);
//         *i += 1;
//     }
//     else
//     {
//         t_token *new_token = lst_new(ft_my_strdup(&cmd[*i], 1, tool), tool);
//         if (new_token)
//             new_token->type = type; // Set token type
//         lst_add_back(token, new_token);
//     }
// }