/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:21:18 by sgmih             #+#    #+#             */
/*   Updated: 2025/04/23 18:07:06 by sgmih            ###   ########.fr       */
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

/******************************************** start garbage collect *********************************************/
t_garbcoll	*new_garbcoll(void *value)
{
	t_garbcoll	*token;

	token = (t_garbcoll *)malloc(sizeof(t_garbcoll));
	if (!token)
		return (NULL);
	token->ptr = value;
	token->next = NULL;
	return (token);
}

void	add_to_grbg(t_garbcoll **head, void *value)
{
	t_garbcoll	*tmp;
	t_garbcoll	*token;

	token = new_garbcoll(value);
	if (!head || !token)
	{
		if (!token)
			write(2, "minishell$> : malloc error\n", 27);
		return ;
	}
	if (!*head)
	{
		*head = token;
		return ;
	}
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = token;
		return ;
	}
}

void	clear_garbcoll(t_garbcoll *head)
{
	t_garbcoll	*token;

	if (!head)
		return ;
	token = head;
	while (head)
	{
		head = head ->next;
		free(token->ptr);
		free(token);
		token = head;
	}
}
// test by amine hhhh 

// void clear_garbcoll(t_garbcoll **head)
// {
//     t_garbcoll *token;
//     t_garbcoll *next;
//     if (!head || !*head)
//         return;
//     token = *head;
//     while (token)
//     {
//         next = token->next;
//         if (token->ptr)
//             free(token->ptr);
//         free(token);
//         token = next;
//     }
//     *head = NULL;
// }
/******************************************** end garbage collect *********************************************/


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

t_token	*lst_new(void *str, t_tool *tool)
{
	t_token	*token;

	if (!str)
		return (NULL);
	token = (t_token *) malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	add_to_grbg(&tool->grbg, token);
	token->value = str;
	init_type(&token);
	token->next = NULL;
	return (token);
}

/******************************************** function libft *********************************************/

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
/******************************************** function libft *********************************************/


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

void	lst_add_back(t_token **head, t_token *token)
{
	t_token	*tmp;

	if (!head || !token)
	{
		if (!token)
			write(2, "minishell$> : malloc error\n", 27);
		return ;
	}
	if (!*head)
	{
		*head = token;
		(*head)->next = NULL;
		return ;
	}
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = token;
		token->next = NULL;
		return ;
	}
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

t_token	*lastone(t_token *head)
{
	if (!head)
		return (NULL);
	while (head->next)
		head = head->next;
	return (head);
}

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
	return (token); // just for while 
}

t_tree	*parsing_input(char *line, t_tool *tool)
{
	t_token	*token;
	
	token = tokens_lst(line, tool);
	print_list(token);

	return (NULL);
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

