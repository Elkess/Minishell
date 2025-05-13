/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:35:16 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/13 15:34:48 by sgmih            ###   ########.fr       */
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


const char *node_type_to_str(t_node_type type)
{
    if (type == NODE_COMMAND) return "NODE_COMMAND";
    if (type == NODE_PIPE)    return "NODE_PIPE";
    if (type == NODE_AND)     return "NODE_AND";
    if (type == NODE_OR)      return "NODE_OR";
    return "UNKNOWN_NODE_TYPE";
}

const char *redir_type_to_str(t_redir_type type)
{
    if (type == REDIR_IN)     return "< (REDIR_IN)";
    if (type == REDIR_OUT)    return "> (REDIR_OUT)";
    if (type == REDIR_APPEND) return ">> (REDIR_APPEND)";
    if (type == REDIR_HEREDOC)return "<< (REDIR_HEREDOC)";
    return "NONE (REDIR_NONE)";
}

void print_redir(t_redir *redir)
{
    if (!redir)
    {
        printf("  No redirections.\n");
        return;
    }

    while (redir)
    {
        printf("  Redirection:\n");
        printf("    Index     : %zu\n", redir->index);
        printf("    Type      : %s\n", redir_type_to_str(redir->type));
        printf("    File      : %s\n", redir->file ? redir->file : "(null)");
        printf("    FD        : %d\n", redir->fd);
        printf("    Flag      : %d\n", redir->flag);
        redir = redir->next;
    }
}

void print_tree(t_tree *node, int depth)
{
    if (!node)
    {
        printf("%*s[NULL node]\n", depth * 2, "");
        return;
    }

    const char *indent = "  ";
    for (int i = 0; i < depth; ++i)
        printf("%s", indent);

    printf("Node Type: %s\n", node_type_to_str(node->type));

    // Print command if it's a command node
    if (node->type == NODE_COMMAND)
    {
        for (int i = 0; node->cmd && node->cmd[i]; ++i)
        {
            for (int j = 0; j < depth; ++j)
                printf("%s", indent);
            printf("  cmd[%d] = %s\n", i, node->cmd[i]);
        }
    }

    // Print redirections
    for (int i = 0; i < depth; ++i)
        printf("%s", indent);
    printf("  Redirections:\n");
    print_redir(node->redirs);

    // Print ambiguity
    for (int i = 0; i < depth; ++i)
        printf("%s", indent);
    printf("  Is Ambiguous: %d\n", node->is_ambiguous);

    // Recurse into left and right nodes
    for (int i = 0; i < depth; ++i)
        printf("%s", indent);
    printf("Left:\n");
    print_tree(node->left, depth + 1);

    for (int i = 0; i < depth; ++i)
        printf("%s", indent);
    printf("Right:\n");
    print_tree(node->right, depth + 1);
}


/******************************************** end print tokens *********************************************/

t_redir *last_node(t_redir *redirs)
{
    t_redir *curr;

    curr = redirs;
    if (!curr)
        return (NULL);
    while (curr->next)
        curr = curr->next;
    return (curr);
}

void    add_redir(t_redir **redirs, t_redir *new_redir)
{
    t_redir *last_redir;

    if (!new_redir)
        return ;
    if (!*redirs)
    {
        *redirs = new_redir;
    }
    else
    {
        last_redir = last_node(*redirs);
        last_redir->next = new_redir;
    }
}

t_redir *create_redir(t_token *token, t_tool *tool, size_t index)
{
    t_redir	*redir;

    redir = (t_redir *) malloc(sizeof(t_redir));
    if (!redir)
        return (NULL);
    add_to_grbg(&tool->grbg, redir);
    
    redir->index = index;
    //redir->type = token->type;
    // Map token types to redirection types
    if (token->type == TOKEN_REDIR_IN)
        redir->type = REDIR_IN;
    else if (token->type == TOKEN_REDIR_OUT)
        redir->type = REDIR_OUT;
    else if (token->type == TOKEN_REDIR_APPEND)
        redir->type = REDIR_APPEND;
    else if (token->type == TOKEN_REDIR_HEREDOC)
        redir->type = REDIR_HEREDOC;
    else
        redir->type = REDIR_NONE;
    redir->file = token->next->value;
    redir->fd = -1;
    redir->flag = 0;
    redir->next = NULL;

    return (redir);
}


t_redir *redir(t_token **input, t_tool *tool)
{
    t_redir	*redir;
    size_t redir_index;

    redir = NULL;
    redir_index = 0;
    while (*input && ((*input)->type == TOKEN_REDIR_IN || (*input)->type == TOKEN_REDIR_OUT ||
            (*input)->type == TOKEN_REDIR_APPEND || (*input)->type == TOKEN_REDIR_HEREDOC))
    {
        add_redir(&redir, create_redir(*input, tool, redir_index));
        *input = (*input)->next;
    }
    return (redir);
}

// Creates a new t_tree node
t_tree  *create_tree_node(t_node_type type, t_tool *tool)
{
    t_tree *node;

    node = (t_tree *) malloc(sizeof(t_tree));
    if (!node)
        return (NULL);
    add_to_grbg(&tool->grbg, node);
    node->type = type;
    node->cmd = NULL;
    node->redirs = NULL;
    node->is_ambiguous = 0;
    node->left = NULL;
    node->right = NULL;
    return (node);
}

/* Modified print function to show the index */
void print_redirs(t_redir *redirs) {
    t_redir *curr = redirs;
    const char *type_names[] = {"REDIR_IN", "REDIR_OUT", "REDIR_APPEND", "REDIR_HEREDOC", "REDIR_NONE"};
    
    if (!curr) {
        printf("No redirections found.\n");
        return;
    }
    
    printf("==== REDIRECTIONS ====\n");
    while (curr) {
        printf("Index: %zu, Type: %s, File: %s, FD: %d, Flag: %d\n", 
               curr->index,
               type_names[curr->type], 
               curr->file ? curr->file : "(null)", 
               curr->fd, 
               curr->flag);
        curr = curr->next;
    }
    printf("=====================\n");
}


t_tree	*parsing_input(char *line, t_tool *tool)
{
	t_token	*token;
    t_redir	*redirs;
    //t_tree	*tree;
    int i;
    
    tool->quoted = 0;
    tool->anderr = 0;
    tool->paren = 0;

    i = 0;
    while (line && line[i])
    {
        hundel_quotes_paren(tool, line[i]);
        i++;
    }
	
	token = tokens_lst(line, tool);
	print_token_list(token);

    // puts("\n");
    // // Process redirections
    // t_token *current_token = token;
    // redirs = redir(&current_token, tool);
    
    // puts("\n");
    // print_redirs(redirs);
    
    // Build AST
    //tree = build_tree(&token, tool);
    //print_tree(tree, 1);
	return (NULL);
}
