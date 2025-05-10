/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:35:16 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/10 09:19:05 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"

/******************************************** start print tokens *********************************************/

// Helper function to convert token type to string for readable output
static const char *get_token_type_name(t_token_type type)
{
    switch (type)
    {
        case TOKEN_WORD: return "WORD";
        case TOKEN_PIPE: return "PIPE";
        case TOKEN_AND: return "AND";
        case TOKEN_OR: return "OR";
        case TOKEN_SPACE: return "SPACE";
        case TOKEN_REDIR_IN: return "REDIR_IN";
        case TOKEN_REDIR_OUT: return "REDIR_OUT";
        case TOKEN_REDIR_APPEND: return "REDIR_APPEND";
        case TOKEN_REDIR_HEREDOC: return "REDIR_HEREDOC";
        case TOKEN_PAREN_OPEN: return "PAREN_OPEN";
        case TOKEN_PAREN_CLOSE: return "PAREN_CLOSE";
        case TOKEN_SINGL_AND: return "SINGL_AND";
        case TOKEN_SINGLE_QUOTE: return "SINGLE_QUOTE";
        case TOKEN_DOUBLE_QUOTE: return "DOUBLE_QUOTE";
        case TOKEN_DOLLAR: return "DOLLAR";
        case TOKEN_WILDCARD: return "WILDCARD";
        case TOKEN_SINGLE_QUOTED_WORD: return "SINGLE_QUOTED_WORD";
        case TOKEN_DOUBLE_QUOTED_WORD: return "DOUBLE_QUOTED_WORD";
        case TOKEN_FILERED_OUT: return "FILERED_OUT";
        case TOKEN_FILERED_IN: return "FILERED_IN";
        case TOKEN_FILERED_APPEND: return "FILERED_APPEND";
        case TOKEN_FILERED_HEREDOC: return "FILERED_HEREDOC";
        default: return "UNKNOWN";
    }
}

// Function to print all tokens in the list, with spaces made visible as underscores
static void print_token_list(t_token *head)
{
    int i = 0;
    while (head)
    {
        printf("Token %d:", i++);
        printf("  Value   : ");
        if (head->value)
        {
            for (int j = 0; head->value[j]; j++)
            {
                if (head->value[j] == ' ')
                    printf("_");  // Replace space with underscore for visibility
                else
                    printf("%c", head->value[j]);
            }
        }
        else
            printf("NULL");
        printf("  Type    : %s", get_token_type_name(head->type));
        printf("  Priority: %d\n", head->priority);
        head = head->next;
    }
}

static void print_tool(t_tool tool)
{
    printf("t_tool struct:\n");
    printf("  paren   = %d\n", tool.paren);
    printf("  quoted  = %d\n", tool.quoted);
    printf("  anderr  = %d\n", tool.anderr);
    printf("  err     = %d\n", tool.err);
}
/******************************************** end print tokens *********************************************/

t_tree	*parsing_input(char *line, t_tool *tool)
{
	t_token	*token;
	
	token = tokens_lst(line, tool);
	print_token_list(token);
	print_tool(*tool);

	return (NULL);
}
