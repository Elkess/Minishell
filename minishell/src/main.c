/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:21:18 by sgmih             #+#    #+#             */
/*   Updated: 2025/04/29 08:54:34 by sgmih            ###   ########.fr       */
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
		printf("----------\n");
		printf("Value: %s\n", node->value);
		printf("Value: %d\n", node->priority);
		printf("Type: %s (%d)\n", get_token_type_name(node->type), node->type);
		printf("----------\n");
		node = node->next;
	}
}

// Helper function to convert t_redir_type to string
static const char *redir_type_to_string(t_redir_type type)
{
    switch (type)
    {
        case REDIR_IN:      return "REDIR_IN";
        case REDIR_OUT:     return "REDIR_OUT";
        case REDIR_APPEND:  return "REDIR_APPEND";
        case REDIR_HEREDOC: return "REDIR_HEREDOC";
        case REDIR_NONE:    return "REDIR_NONE";
        default:            return "UNKNOWN";
    }
}

// Function to print the t_redir list
void print_redir_list(t_redir *redir)
{
    if (!redir)
    {
        printf("No redirections found.\n");
        return;
    }
    while (redir)
    {
        printf("{index: %zu, type: %s, file: \"%s\", next: %s}\n",
               redir->index,
               redir_type_to_string(redir->type),
               redir->file ? redir->file : "NULL",
               redir->next ? "..." : "NULL");
        redir = redir->next;
    }
}

/******************************************** end print tokens *********************************************/

// Helper function to create a new redirection node
static t_redir *create_redir_node(size_t index, t_redir_type type, char *file, t_tool *tool)
{
    t_redir *node;
	
	node = (t_redir *) malloc(sizeof(t_redir));
    if (!node)
        return (NULL);
	add_to_grbg(&tool->grbg, node);
    node->index = index;
    node->type = type;
    node->file = file;
    node->next = NULL;
    return (node);
}

t_redir	*update_lst(t_token *token, t_tool *tool)
{
	t_redir *redir_head;
	t_redir *current;
	t_redir_type redir_type;
	size_t index;
	char *file;

	redir_head = NULL;
	current = NULL;
	file = NULL;
	index = 0;

	while (token)
	{
		redir_type = -1;
		// Map token type to redir type and check for corresponding file token

		if (token->type == REDIR_HEREDOC)
        {
            redir_type = REDIR_HEREDOC;
            if (token->next && token->next->type == TOKEN_FILERED_HEREDOC)
            {
                file = token->next->value;
                token = token->next; // Skip file token
            }
        }
        else if (token->type == REDIR_IN)
        {
            redir_type = REDIR_IN;
            if (token->next && token->next->type == TOKEN_FILERED_IN)
            {
                file = token->next->value;
                token = token->next; // Skip file token
            }
        }
        else if (token->type == REDIR_OUT)
        {
            redir_type = REDIR_OUT;
            if (token->next && token->next->type == TOKEN_FILERED_OUT)
            {
                file = token->next->value;
                token = token->next; // Skip file token
            }
        }
        else if (token->type == REDIR_APPEND)
        {
            redir_type = REDIR_APPEND;
            if (token->next && token->next->type == TOKEN_FILERED_APPEND)
            {
                file = token->next->value;
                token = token->next; // Skip file token
            }
        }

		if (redir_type != REDIR_NONE)
        {
            t_redir *new_node = create_redir_node(index++, redir_type, file, tool);
            if (!new_node)
                return NULL; // Cleanup needed in real implementation

            if (!redir_head)
            {
                redir_head = new_node;
                current = new_node;
            }
            else
            {
                current->next = new_node;
                current = new_node;
            }
        }
        token = token->next;
	}
	return (redir_head);
	
}

t_token	*check_token(t_token **token, t_tool *tool)
{
	if (pars_err(token, tool))
		return (NULL);
	//update_lst(&token, tool);
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

