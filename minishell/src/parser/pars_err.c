/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_err.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 08:46:52 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/23 08:05:57 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_redirection(int type)
{
	if (type == 5 || type == 6 || type == 7
		|| type == 8)
		return (1);
	return (0);
}

static int	condition(t_token *token)
{
	
	if (is_redirection(token->type))
	{
		
		if (!token->next)
		{
			puts("condi 1-4");
			return (3); // e.g., ">", "<", ">>", "<<"
		}
		if (token->next->type == TOKEN_PIPE || token->next->type == TOKEN_AND || token->next->type == TOKEN_OR || is_redirection(token->next->type))
		{
			puts("condi 2-4");
			return (2); // e.g., "> |", "< )"
		}
		if (token->next->type == TOKEN_PAREN_OPEN || token->next->type == TOKEN_PAREN_CLOSE)
		{  
			puts("condi 3-4");
			return (2); // e.g., "> (", "< ("
		}
	}
	
	if (token->priority > 0 && !token->next && token->type != TOKEN_PAREN_CLOSE)
	{
		puts("condi 1");
		return (1); // e.g., "cmd |" or "cmd &&"
	}

	if (token->next && (token->type == TOKEN_PIPE || token->type == TOKEN_AND || token->type == TOKEN_OR) &&
	(token->next->type == TOKEN_PIPE || token->next->type == TOKEN_AND || token->next->type == TOKEN_OR || token->next->type == TOKEN_PAREN_CLOSE))
	{
		puts("condi 2 extended");
		return (2); // e.g., "||", "&& |", "| )"
	}

	if (token->type == TOKEN_PAREN_CLOSE && token->next && token->next->type == TOKEN_WORD)
	{
		puts("condi 3");
		return (2); // e.g., ") cmd"
	}

	if (token->type == TOKEN_PAREN_OPEN && token->next &&
		(token->next->type == TOKEN_PIPE || token->next->type == TOKEN_AND || token->next->type == TOKEN_OR))
	{
		puts("condi 5");
		return (2); // e.g., "( |"
	}

	if ((token->type == TOKEN_WORD || token->type == TOKEN_DOUBLE_QUOTED_WORD || token->type == TOKEN_SINGLE_QUOTED_WORD 
		|| token->type == TOKEN_PAREN_CLOSE) && token->next && token->next->type == TOKEN_PAREN_OPEN)
	{
		puts("condi 6");
		return (2); // e.g., "cmd (" or ") ("
	}

	if (token->next && token->type == TOKEN_PAREN_CLOSE && token->next->type == TOKEN_PAREN_OPEN)
	{
		puts("condi 7");
		return (1); // e.g., ") ("
	}

	if (token->type == TOKEN_PAREN_OPEN && !token->next)
	{
		puts("condi 8");
		return (1); // e.g., "("
	}

	if (token->type == TOKEN_PAREN_OPEN && token->next && token->next->type == TOKEN_PAREN_CLOSE)
	{
		puts("condi 9");
		return (1); // e.g., "()"
	}

	if ((token->type <= 0 && token->next && token->next->type == TOKEN_PAREN_OPEN))
		return (2);
			
	return (0);
}

int	pars_err_utils(t_token *token, t_tool *tool)
{
	int	has_command;
	int	expecting_redirection_arg;
	int	err_code;

	has_command = 0;
	expecting_redirection_arg = 0;
	while (token)
	{
		err_code = condition(token);
		if (err_code)
		{
			write(2, "minishell$> : syntax error near unexpected token `", 51);
			if (err_code == 1)
				ft_putstr_fd(token->value, 2);
			else if (err_code == 2 && token->next)
				ft_putstr_fd(token->next->value, 2);
			else if (err_code == 3)
				ft_putstr_fd("newline", 2);
			write(2, "'\n", 2);
			tool->err = 2;
			return (1);
		}
		token = token->next;
	}
	return (0);
}

int	pars_err(t_token **token, t_tool *tool)
{
	t_token	*tmp;

	tmp = *token;
	if (tmp && (tmp->type == 1 || tmp->type == 2 || tmp->type == 3))
	{
		write(2, "minishell$> : syntax error near unexpected token `", 50);
		ft_putstr_fd(tmp->value, 2);
		write(2, "'\n", 2);
		return (tool->err = 2, 1);
	}
	if (tool->paren || tool->quoted || tool->anderr == 1)
	{
		if (tool->paren)
			write(2,
				"minishell$> : syntax error near unexpected token `)'\n", 53);
		else if (tool->anderr == 1)
			write(2,
				"minishell$> : syntax error near unexpected token `&'\n", 53);
		else
			write(2, "minishell$> : syntax error quotes\n", 35);
		return (tool->err = 2, 1);
	}
	if (pars_err_utils(tmp, tool))
		return (1);
	return (0);
}
