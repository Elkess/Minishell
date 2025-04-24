/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:21:18 by sgmih             #+#    #+#             */
/*   Updated: 2025/04/24 10:33:21 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_get_prompt(void)
{
	char	*prompt;

	prompt = readline("minishell$> ");
	if (prompt && prompt[0])
		add_history(prompt);
	if (!prompt)
	{
		printf("exit\n");
		exit(0);
	}
	return (prompt);
}

/******************************************** start print tokens *********************************************/

const char *get_token_type_name(t_token_type type)
{
	switch (type)
	{
		case TOKEN_WORD:           return "WORD";
		case TOKEN_PIPE:           return "PIPE";
		case TOKEN_AND:            return "AND";
		case TOKEN_OR:             return "OR";
		case TOKEN_REDIR_IN:       return "REDIR_IN";
		case TOKEN_REDIR_OUT:      return "REDIR_OUT";
		case TOKEN_REDIR_APPEND:   return "REDIR_APPEND";
		case TOKEN_REDIR_HEREDOC:  return "REDIR_HEREDOC";
		case TOKEN_PAREN_OPEN:     return "PAREN_OPEN";
		case TOKEN_PAREN_CLOSE:    return "PAREN_CLOSE";
		case TOKEN_SINGL_AND:      return "SINGLE_AND";
		case TOKEN_FILERED_OUT:    return "FILERED_OUT";
		case TOKEN_FILERED_IN:     return "FILERED_IN";
		case TOKEN_FILERED_APPEND: return "FILERED_APPEND";
		case TOKEN_FILERED_HEREDOC:return "FILERED_HEREDOC";
		default:                   return "UNKNOWN";
	}
}

void print_list(t_token *node)
{
	while (node)
	{
		printf("Value: %s\n", node->value);
		printf("Type: %s (%d)\n", get_token_type_name(node->type), node->type);
		printf("----------\n");
		node = node->next;
	}
}

/******************************************** end print tokens *********************************************/

/******************************************** function libft **********************************************/

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
/******************************************** function libft *********************************************/


// static int	pars_err_utils(t_token *token, t_tool *tool)
// {
// 	int		lst_token;
// 	t_token	*token_heredoc;
// 	lst_token = 0;
// 	token_heredoc = NULL;
// 	while (token)
// 	{
// 		if (token->type > 0 && !is_redarection(token->type))
// 			lst_token = token->type;
// 		if (condtion(token) || (lst_token && lst_token == TOKEN_PAREN_CLOSE && token->type == 0))
// 		{
// 			tool->err = 2;
// 			write(2, "minishell$> : syntax error near unexpected token `", 49);
// 			ft_putstr_fd(token->value, 2);
// 			write(2, "'\n", 2);
// 			ft_clearhd(token_heredoc);
// 			return (1);
// 		}
// 		if (token->type == 14 && token->next && heredoc(tool, &(token->next->value), &token_heredoc))
// 		{
// 			ft_clearhd(token_heredoc);
// 			write(2, "syntax error in token_heredoc \n", 31);
// 			return (1);
// 		}
// 		token = token->next;
// 	}
// }

int	pars_err(t_token **token, t_tool *tool)
{
	t_token	*tmp;

	tmp = *token;
	if (tmp && (tmp->type == 1 || tmp->type == 2 || tmp->type == 3 || tmp->type == 4 || tmp->type == 5
		|| tmp->type == 6 || tmp->type == 7 || tmp->type == 9 || tmp->type == 10))
	{
		tool->err = 2;
		write(2, "minishell$> : syntax error near unexpected token `", 50);
		ft_putstr_fd(tmp->value, 2);
		write(2, "'\n", 2);
		return (1);
	}
	// if (pars_err_utils(tmp, tool))
	// 	return (1);
	if (tool->paren|| tool->quoted || tool->anderr == 1)
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

t_token	*check_token(t_token **token, t_tool *tool)
{
	if (pars_err(token, tool))
		return (NULL);
	// update_lst(node, tool);
	// redarection_join_arg(node, tool);
	// trime(*node, tool);
	return (*token);
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_tree	*tree;
	t_tool	tool;
	
	(void)ac;
	(void)av;
	(void)env;

	while (1)
	{
		line = ft_get_prompt();
		printf("Line input: %s\n", line);

		tree = parsing_input(line, &tool);

		//handle_input(line);
		free(line);
	}
	return (0);
}

