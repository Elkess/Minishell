/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:35:16 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/19 10:59:21 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


/******************************************** start print tokens *********************************************/

// ANSI color codes
#define ANSI_RESET "\033[0m"
#define ANSI_CYAN "\033[36m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_GREEN "\033[32m"
#define ANSI_MAGENTA "\033[35m"
#define ANSI_RED "\033[31m"

// Get node type string
const char *get_node_type_str(t_node_type type) {
    switch (type) {
        case NODE_COMMAND: return ANSI_CYAN "CMD" ANSI_RESET;
        case NODE_PIPE: return ANSI_YELLOW "PIPE" ANSI_RESET;
        case NODE_OR: return ANSI_YELLOW "||" ANSI_RESET;
        case NODE_AND: return ANSI_YELLOW "&&" ANSI_RESET;
        case NODE_PARENTHS: return ANSI_CYAN "()" ANSI_RESET;
        default: return ANSI_RED "UNKNOWN" ANSI_RESET;
    }
}

// Get redirection type string
const char *get_redir_type_str(t_redir_type type) {
    switch (type) {
        case REDIR_IN: return ANSI_MAGENTA "< (REDIR_IN)" ANSI_RESET;
        case REDIR_OUT: return ANSI_MAGENTA "> (REDIR_OUT)" ANSI_RESET;
        case REDIR_APPEND: return ANSI_MAGENTA ">> (REDIR_APPEND)" ANSI_RESET;
        case REDIR_HEREDOC: return ANSI_MAGENTA "<< (REDIR_HEREDOC)" ANSI_RESET;
        default: return ANSI_RED "Unknown (REDIR_NONE)" ANSI_RESET;
    }
}

// Print command, parentheses, and redirection details for a node
void print_node_details(t_tree *node, char *prefix, int is_last) {
    if (node->type == NODE_COMMAND) {
        // Print command node header
        printf("%s%s %sCommand Node: %u%s\n", prefix, is_last ? "└──" : "├──", 
               ANSI_CYAN, node->type, ANSI_RESET);

        // Print command
        printf("%s%s  %sCmd: %s", prefix, is_last ? "    " : "│   ", ANSI_GREEN, ANSI_RESET);
        if (node->cmd && node->cmd[0]) {
            for (int i = 0; node->cmd[i]; i++)
                printf("%s ", node->cmd[i]);
            printf("\n");
        } else {
            printf("(none)\n");
        }

        // Print redirections
        printf("%s%s  %sRedirs: %s", prefix, is_last ? "    " : "│   ", ANSI_MAGENTA, ANSI_RESET);
        if (node->redirs) {
            printf("\n");
            t_redir *curr = node->redirs;
            while (curr) {
                printf("%s%s    %sIndex     : %s%zu\n", prefix, is_last ? "    " : "│   ", 
                       ANSI_YELLOW, ANSI_RESET, curr->index);
                printf("%s%s    %sType      : %s%s\n", prefix, is_last ? "    " : "│   ", 
                       ANSI_YELLOW, ANSI_RESET, get_redir_type_str(curr->type));
                printf("%s%s    %sFile      : %s%s\n", prefix, is_last ? "    " : "│   ", 
                       ANSI_YELLOW, ANSI_RESET, curr->file ? curr->file : "(null)");
                printf("%s%s    %sFD        : %s%d\n", prefix, is_last ? "    " : "│   ", 
                       ANSI_YELLOW, ANSI_RESET, curr->fd);
                curr = curr->next;
            }
        } else {
            printf("(none)\n");
        }
    } else if (node->type == NODE_PARENTHS) {
        // Print parentheses node header
        printf("%s%s %sOperator Node: %s%s\n", prefix, is_last ? "└──" : "├──", 
               ANSI_CYAN, get_node_type_str(node->type), ANSI_RESET);

        // Print redirections
        printf("%s%s  %sRedirs: %s", prefix, is_last ? "    " : "│   ", ANSI_MAGENTA, ANSI_RESET);
        if (node->redirs) {
            printf("\n");
            t_redir *curr = node->redirs;
            while (curr) {
                printf("%s%s    %sIndex     : %s%zu\n", prefix, is_last ? "    " : "│   ", 
                       ANSI_YELLOW, ANSI_RESET, curr->index);
                printf("%s%s    %sType      : %s%s\n", prefix, is_last ? "    " : "│   ", 
                       ANSI_YELLOW, ANSI_RESET, get_redir_type_str(curr->type));
                printf("%s%s    %sFile      : %s%s\n", prefix, is_last ? "    " : "│   ", 
                       ANSI_YELLOW, ANSI_RESET, curr->file ? curr->file : "(null)");
                printf("%s%s    %sFD        : %s%d\n", prefix, is_last ? "    " : "│   ", 
                       ANSI_YELLOW, ANSI_RESET, curr->fd);
                curr = curr->next;
            }
        } else {
            printf("(none)\n");
        }
    } else {
        // Print other operator nodes
        printf("%s%s %sOperator Node: %s%s\n", prefix, is_last ? "└──" : "├──", 
               ANSI_YELLOW, get_node_type_str(node->type), ANSI_RESET);
    }
}

// Get annotation for a node
void get_node_annotation(t_tree *node, t_tree *parent, int is_left, char *buffer, size_t buf_size) {
    char node_str[256];
    snprintf(node_str, sizeof(node_str), "%s", node->type == NODE_COMMAND ? "CMD" : get_node_type_str(node->type));
    
    if (!parent) {
        snprintf(buffer, buf_size, "%s// Root%s", ANSI_GREEN, ANSI_RESET);
    } else {
        char parent_str[256];
        snprintf(parent_str, sizeof(parent_str), "%s", parent->type == NODE_COMMAND ? "CMD" : get_node_type_str(parent->type));
        snprintf(buffer, buf_size, "%s// %s subtree of %s%s", ANSI_GREEN, is_left ? "Left" : "Right", parent_str, ANSI_RESET);
    }
}

// Recursive function to print tree
void print_tree_recursive(t_tree *node, char *prefix, int is_last, t_tree *parent, int is_left, int level) {
    if (!node) return;

    // Print node details
    print_node_details(node, prefix, is_last);

    // Print annotation
    char annotation[512];
    get_node_annotation(node, parent, is_left, annotation, sizeof(annotation));
    printf("%s%s%s\n", prefix, is_last ? "    " : "│   ", annotation);

    // Prepare prefix for children
    char new_prefix[256];
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_last ? "    " : "│   ");

    // Recursively print children
    if (node->left || node->right) {
        if (node->left)
            print_tree_recursive(node->left, new_prefix, node->right == NULL, node, 1, level + 1);
        if (node->right)
            print_tree_recursive(node->right, new_prefix, 1, node, 0, level + 1);
    }
}

// Main function to print tree
void print_tree(t_tree *node) {
    if (!node) {
        printf("%sEmpty tree%s\n", ANSI_RED, ANSI_RESET);
        return;
    }
    print_tree_recursive(node, "", 1, NULL, 0, 0);
}


//////************ tokens
/*
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
        printf("\033[38;5;1m => Token %d:", i++);
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
        printf("  Priority: %d\033[0m \n", head->priority);
        head = head->next;
    }
}
// #include <stdio.h>


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

    // Recurse on children
    print_tree_redirs(tree->left, indent + 1);
    print_tree_redirs(tree->right, indent + 1);
}
*/
/********************************************************************************************************/

void	init_struct_tool(t_tool *tool)
{
	tool->quoted = 0;
	tool->anderr = 0;
	tool->paren = 0;
	tool->grbg = NULL;
}

t_tree	*parsing_input(char *line, t_tool *tool) 
{
	t_token	*token;
	t_tree	*tree;
	int		i;

	i = 0;
	init_struct_tool(tool);
	while (line && line[i])
	{
		hundel_quotes_paren(tool, line[i]);
		i++;
	}
	token = tokens_lst(line, tool); // TODO: not use priority in token
	if (pars_err(&token, tool))
		return (NULL);
	tree = ft_tree(&token, tool);
	//print_tree(tree);
	return (tree);
}
