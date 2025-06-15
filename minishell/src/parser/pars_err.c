/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_err.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 08:46:52 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/15 10:24:00 by melkess          ###   ########.fr       */
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

static int	condition_utils(t_token *token)
{
	if ((token->type == 10 && token->next && token->next->type == 0)
		|| (token->type == 9 && token->next && (token->next->type >= 1
				&& token->next->type <= 3))
		|| ((token->type == 0 || token->type == 16 || token->type == 17
				|| token->type == 10) && token->next && token->next->type == 9)
		|| (token->type <= 0 && token->next && token->next->type == 9))
		return (2);
	if ((token->next && token->type == 10 && token->next->type == 9)
		|| (token->type == 9 && !token->next)
		|| (token->type == 9 && token->next && token->next->type == 10))
		return (1);
	return (0);
}

static int	condition(t_token *token)
{
	if (is_redirection(token->type))
	{
		if (!token->next)
			return (3);
		if ((token->next->type >= 1 && token->next->type <= 3)
			|| is_redirection(token->next->type)
			|| token->next->type == 9 || token->next->type == 10)
			return (2);
	}
	if (token->priority > 0 && !token->next && token->type != 10)
		return (1);
	if (token->next && (token->type >= 1 && token->type <= 3)
		&& (token->next->type >= 1 && token->next->type <= 3
			|| token->next->type == 10))
		return (2);
	return (condition_utils(token));
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
			tool->err = 258;
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
		return (tool->err = 258, 1);
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
		return (tool->err = 258, 1);
	}
	if (pars_err_utils(tmp, tool))
		return (1);
	return (0);
}
