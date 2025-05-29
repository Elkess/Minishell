/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:35:16 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/29 10:40:02 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
        case TOKEN_SINGLE_QUOTED_WORD: return "SINGLE_QUOTED_WORD";
        case TOKEN_DOUBLE_QUOTED_WORD: return "DOUBLE_QUOTED_WORD";
        case TOKEN_HAS_QUOTED: return "TOKEN_HAS_QUOTED";
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
        printf("\033[31mToken \033[0m %d:", i++);
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

#include <stdio.h>

void print_redir_list(t_redir *redir, const char *list_name, int indent)
{
    t_redir *curr = redir;
    int i;

    for (i = 0; i < indent; i++)
        printf("  ");
    printf("%s:\n", list_name);
    if (!curr)
    {
        for (i = 0; i < indent + 1; i++)
            printf("  ");
        printf("(none)\n");
        return;
    }
    while (curr)
    {
        for (i = 0; i < indent + 1; i++)
            printf("  ");
        printf("type: ");
        switch (curr->type)
        {
            case REDIR_IN: printf("REDIR_IN"); break;
            case REDIR_OUT: printf("REDIR_OUT"); break;
            case REDIR_APPEND: printf("REDIR_APPEND"); break;
            case REDIR_HEREDOC: printf("REDIR_HEREDOC"); break;
            case REDIR_NONE: printf("REDIR_NONE"); break;
            default: printf("UNKNOWN"); break;
        }
        printf(", file: %s, index: %zu\n", curr->file ? curr->file : "(null)", curr->index);
        curr = curr->next;
    }
}

void print_tree_redirs(t_tree *tree, int indent)
{
    int i;

    if (!tree)
        return;

    // Print node info
    for (i = 0; i < indent; i++)
        printf("  ");
    printf("Node type: ");
    switch (tree->type)
    {
        case NODE_COMMAND: printf("COMMAND"); break;
        case NODE_PIPE: printf("PIPE"); break;
        case NODE_AND: printf("AND"); break;
        case NODE_OR: printf("OR"); break;
        case NODE_PARENTHS: printf("PARENTHS"); break;
        default: printf("UNKNOWN"); break;
    }
    printf("\n");

    // Print redirections
    print_redir_list(tree->redirs, "redirs", indent + 1);
    print_redir_list(tree->redirs_before, "redirs_before", indent + 1);
    print_redir_list(tree->redirs_after, "redirs_after", indent + 1);

    // Recurse on children
    print_tree_redirs(tree->left, indent + 1);
    print_tree_redirs(tree->right, indent + 1);
}

/********************************************************************************************************/

	
t_tree	*parsing_input(char *line, t_tool *tool)
{
	t_token	*token;
	t_tree	*tree;
	int		i;

	tool->quoted = 0;
	tool->anderr = 0;
	tool->paren = 0;
	tool->err = 0;
	tool->grbg = NULL;
	i = 0;
	while (line && line[i])
	{
		hundel_quotes_paren(tool, line[i]);
		i++;
	}
	token = tokens_lst(line, tool);
	//print_token_list(token);
     
	if (pars_err(&token, tool))
		return (NULL);
    
	tree = ft_tree(&token, tool);
	return (tree);
}
