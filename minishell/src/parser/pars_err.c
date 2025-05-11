/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_err.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 08:46:52 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/11 10:59:52 by sgmih            ###   ########.fr       */
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
	if (type == 5 || type == 6 || type == 7 || type == 8)
		return (1);
	return (0);
}

static int	err_condtion(t_token *token)
{
    t_token *next_non_space;
	// close par
	if (token->priority > 0 && !token->next && token->type != prio_close_par) {
        //printf("Condition 1 triggered for token '%s'\n", token->value);
        return (1);
    }

    if (token->type == prio_close_par && token->next && token->next->type == 0) {
        //printf("Condition 2 triggered for token '%s'\n", token->value);
        return (2); // Error on next token (e.g., 'cmd' in '(ls) cmd')
    }


    // Skip spaces and check operators
    if (token->next)
    {
        next_non_space = token->next;
        while (next_non_space && next_non_space->type == TOKEN_SPACE)
            next_non_space = next_non_space->next;

        if (next_non_space && (token->type == prio_pipe || token->type == prio_and || token->type == prio_or) &&
            (next_non_space->type == prio_pipe || next_non_space->type == prio_and || next_non_space->type == prio_or))
        {
            //printf("Condition 4 triggered for token '%s' and next token '%s'\n", token->value, next_non_space->value);
            return (2);
        }
    }

    if (token->next && token->type > 0 && token->next->type > 0 &&
        token->type != prio_close_par && token->next->type != prio_open_par &&
        !is_redirection(token->type) && !is_redirection(token->next->type)) {
        //printf("Condition 5 triggered for token '%s' and next token '%s'\n", token->value, token->next->value);
        return (1);
    }

    // Redirection without a file/delimiter
    if (is_redirection(token->type) && (!token->next || token->next->type >= 0)) {
        //printf("Condition 6 triggered for token '%s'\n", token->value);
        return (1);
    }

	return (0);
}

static t_token *last_one(t_token *token)
{
    t_token *current = token;
    t_token *last_non_space = NULL;

    while (current)
    {
        if (current->type != TOKEN_SPACE)
            last_non_space = current;
        current = current->next;
    }
    printf("last token: %s\n", last_non_space->value);
    return (last_non_space);
}

static int	pars_err_utils(t_token *token, t_tool *tool)
{
    t_token *last_token;
    int i;

    i = 0;
    last_token = last_one(token); 
    while(token)
    {
        if (token->type == TOKEN_SPACE)
        {
            token = token->next;
            continue ;
        }
        if (token->type > 0 && !is_redirection(token->type))
            i = token->type;
        if (err_condtion(token) || (i && i == 10 && token->type == 0))
        {
			write(2, "minishell$> : syntax error near unexpected token `", 50);
            if (is_redirection(token->type) && !token->next)
                write(2, "newline", 7);
			else if (err_condtion(token) == 2 && token->next)
                ft_putstr_fd(last_token->value, 2);
            else
                ft_putstr_fd(token->value, 2);
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