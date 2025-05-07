/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_err.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 08:46:52 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/03 16:52:10 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"

static void	ft_putstr_fd(char *s, int fd)
{
	unsigned int	i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
		i++;
	write(fd, s, i);
}

static	int	is_redirection(int type)
{
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT || type == TOKEN_REDIR_APPEND
		|| type == TOKEN_REDIR_HEREDOC)
		return (1);
	return (0);
}

static int	condtion(t_token *token)
{
		
	if (token->priority > 0 && !token->next && token->type != prio_close_par)
        return (1);

    if (token->next && ((token->type == prio_pipe || token->type == prio_and || token->type == prio_or) &&
        (token->next->type == prio_pipe || token->next->type == prio_and || token->next->type == prio_or)))
        return (2);  // Error on next token

	if (token->next && token->type > 0 && token->next->type > 0 &&
        token->type != prio_close_par && token->next->type != prio_open_par &&
        !is_redirection(token->type) && !is_redirection(token->next->type))
        return (1);

	if (token->type == prio_close_par && token->next && token->next->type == TOKEN_WORD)
        return (2);
	
    if (
        (is_redirection(token->type) && !(token->next->type < 0)) 
        || (token->type <= 0 && token->next && token->next->type == prio_open_par)
		|| (token->type == prio_close_par && token->next && token->next->type == 0)
		|| (token->next && token->type == token->next->type && token->next->type <= 0)
    )
        return (1);
			
	return (0);
}

static void	ft_clear_token_heredoc(t_token *token_heredoc)
{
	while (token_heredoc)
	{
		unlink(token_heredoc->value);
		token_heredoc = token_heredoc->next;
	}
}

static int	pars_err_utils(t_token *token, t_tool *tool)
{
	int		lst_token;
	t_token	*token_heredoc;
	
	lst_token = 0;
	token_heredoc = NULL;
	while (token)
	{//TODO: nxra7 l amine mn hna 
		if (token->type > 0 && !is_redirection(token->type))
			lst_token = token->type;
		if (condtion(token) || (lst_token && lst_token == TOKEN_PAREN_CLOSE && token->type == 0))
		{
			tool->err = 2;
			write(2, "minishell$> : syntax error near unexpected token `", 51);
            if (is_redirection(token->type) && !token->next)
                write(2, "newline", 7);
			else if (condtion(token) == 2 && token->next)
                ft_putstr_fd(token->next->value, 2);
            else
                ft_putstr_fd(token->value, 2);
			write(2, "'\n", 2);
			ft_clear_token_heredoc(token_heredoc);
			return (1);
		}
					
		// if (token->type == 7 && token->next && pras_heredoc(tool, &(token->next->value), &token_heredoc))
		// {
		// 	ft_clear_token_heredoc(token_heredoc);
		// 	write(2, "syntax error in token_redir_heredoc \n", 37);
		// 	return (1);
		// }
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
		tool->err = 2;
		write(2, "minishell$> : syntax error near unexpected token `", 50);
		ft_putstr_fd(tmp->value, 2);
		write(2, "'\n", 2);
		return (1);
	}
	if (pars_err_utils(tmp, tool))
		return (1);
	if (tool->paren || tool->quoted || tool->anderr == 1)
	{
		if (tool->paren)
			write(2, "minishell$> : syntax error near unexpected token `)'\n", 53);
		else if (tool->anderr == 1)
			write(2, "minishell$> : syntax error near unexpected token `&'\n", 53);
		else
			write(2, "minishell$> : syntax error quotes\n", 35);
		tool->err = 2;
		return (1);
	}
	return (0);
}

