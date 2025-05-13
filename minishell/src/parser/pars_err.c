/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_err.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 08:46:52 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/13 15:48:22 by sgmih            ###   ########.fr       */
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

static int is_redirection(int type)
{
    if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT || type == TOKEN_REDIR_APPEND
		|| type == TOKEN_REDIR_HEREDOC)
		return (1);
	return (0);
}

static int condition(t_token *token)
{
    t_token *next = token->next;
    t_token *temp;
    
    // ls |, echo &&, etc....
    if ((token->type == TOKEN_PIPE || token->type == TOKEN_AND || token->type == TOKEN_OR)  && !next)
        return (1); 

    // || &&, etc...
    if (next && (token->type == TOKEN_PIPE || token->type == TOKEN_AND || token->type == TOKEN_OR) &&
                   (next->type == TOKEN_PIPE || next->type == TOKEN_AND || next->type == TOKEN_OR))
        return 2;

    //  Closing paren before word → "(echo) ls"
    if (token->type == TOKEN_PAREN_CLOSE && next && next->type == TOKEN_WORD)
        return 2;

    // redir not befor filename
    if (is_redirection(token->type))
    {
        temp = next;
        while (temp && temp->type == TOKEN_SPACE)
            temp = temp->next;
        if (!temp || !((temp->type) < 0 || temp->type == TOKEN_WORD))
            return (1);
    }
    if (token->type == TOKEN_WORD && next && next->type == TOKEN_PAREN_OPEN)
        return (1);

    return 0;
}

static int pars_err_utils(t_token *token, t_tool *tool)
{
    t_token *lst_token;
    t_token *next;

    while (token)
    {
        if (condition(token))
        {
            lst_token = token;
            write(2, "minishell$> : syntax error near unexpected token `", 50);
            if (is_redirection(lst_token->type) && !token->next)
                write(2, "newline", 7);
            else
                ft_putstr_fd(lst_token->value, 2);
            write(2, "'\n", 2);
            tool->err = 2;
            return (1);
        }
        token = token->next;
    }
    return 0;
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
		tool->err = 2;
		return (1);
	}
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
    if (pars_err_utils(tmp, tool))
		return (1);

	return (0);
}
