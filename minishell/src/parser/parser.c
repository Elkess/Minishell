/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:35:16 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/17 09:37:47 by sgmih            ###   ########.fr       */
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


void print_tree(t_tree *tree, int level)
{
    if (!tree)
        return;

    // Indentation for readability
    for (int i = 0; i < level; i++)
        printf("  ");

    if (tree->type == NODE_COMMAND)
    {
        printf("Command Node: %u\n", tree->type);

        // Print command
        for (int i = 0; i < level; i++)
            printf("  ");
        printf("  Cmd: ");
        if (tree->cmd)
        {
            for (int i = 0; tree->cmd[i]; i++)
                printf("%s ", tree->cmd[i]);
            printf("\n");
        }
        else
            printf("(none)\n");

        // Print before redirections
        for (int i = 0; i < level; i++)
            printf("  ");
        printf("  Redirs Before: ");
        if (tree->redirs_before)
        {
            printf("\n");
            t_redir *curr = tree->redirs_before;
            while (curr)
            {
                for (int i = 0; i < level + 1; i++)
                    printf("  ");
                printf("Index     : %zu\n", curr->index);
                for (int i = 0; i < level + 1; i++)
                    printf("  ");
                printf("Type      : ");
                if (curr->type == REDIR_IN)
                    printf("< (REDIR_IN)\n");
                else if (curr->type == REDIR_OUT)
                    printf("> (REDIR_OUT)\n");
                else if (curr->type == REDIR_APPEND)
                    printf(">> (REDIR_APPEND)\n");
                else if (curr->type == REDIR_HEREDOC)
                    printf("<< (REDIR_HEREDOC)\n");
                else
                    printf("Unknown (REDIR_NONE)\n");
                for (int i = 0; i < level + 1; i++)
                    printf("  ");
                printf("File      : %s\n", curr->file ? curr->file : "(null)");
                for (int i = 0; i < level + 1; i++)
                    printf("  ");
                printf("FD        : %d\n", curr->fd);
                curr = curr->next;
            }
        }
        else
            printf("(none)\n");

        // Print after redirections
        for (int i = 0; i < level; i++)
            printf("  ");
        printf("  Redirs After: ");
        if (tree->redirs_after)
        {
            printf("\n");
            t_redir *curr = tree->redirs_after;
            while (curr)
            {
                for (int i = 0; i < level + 1; i++)
                    printf("  ");
                printf("Index     : %zu\n", curr->index);
                for (int i = 0; i < level + 1; i++)
                    printf("  ");
                printf("Type      : ");
                if (curr->type == REDIR_IN)
                    printf("< (REDIR_IN)\n");
                else if (curr->type == REDIR_OUT)
                    printf("> (REDIR_OUT)\n");
                else if (curr->type == REDIR_APPEND)
                    printf(">> (REDIR_APPEND)\n");
                else if (curr->type == REDIR_HEREDOC)
                    printf("<< (REDIR_HEREDOC)\n");
                else
                    printf("Unknown (REDIR_NONE)\n");
                for (int i = 0; i < level + 1; i++)
                    printf("  ");
                printf("File      : %s\n", curr->file ? curr->file : "(null)");
                for (int i = 0; i < level + 1; i++)
                    printf("  ");
                printf("FD        : %d\n", curr->fd);
                curr = curr->next;
            }
        }
        else
            printf("(none)\n");
    }
    else
    {
        printf("Operator Node: %d\n", tree->type);
    }

    // Recursively print left and right subtrees
    print_tree(tree->left, level + 1);
    print_tree(tree->right, level + 1);
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
    t_redir *redir;
    size_t redir_index;

    redir = NULL;
    redir_index = 0;
    while (*input && ((*input)->type == TOKEN_REDIR_IN || (*input)->type == TOKEN_REDIR_OUT ||
            (*input)->type == TOKEN_REDIR_APPEND || (*input)->type == TOKEN_REDIR_HEREDOC))
    {
        add_redir(&redir, create_redir(*input, tool, redir_index));
        *input = (*input)->next;
        if (*input)
            *input = (*input)->next;
        redir_index++;
    }
    return (redir);
}

t_tree  *create_tree_node(t_node_type type, t_tool *tool)
{
    t_tree *node;

    node = (t_tree *) malloc(sizeof(t_tree));
    if (!node)
        return (NULL);
    add_to_grbg(&tool->grbg, node);
    node->type = type;
    node->cmd = NULL;
    node->redirs_before = NULL;
    node->redirs_after = NULL;
    node->is_ambiguous = 0;
    node->left = NULL;
    node->right = NULL;
    return (node);
}


char **create_cmd_array(t_token **input, t_tool *tool)
{
    int count = 1;
    t_token *current = (*input)->next;
    char **array;
    int i = 0;
    
    while (current && (current->type == TOKEN_WORD || current->type == TOKEN_SINGLE_QUOTED_WORD || 
                       current->type == TOKEN_DOUBLE_QUOTED_WORD || current->type == TOKEN_HAS_QUOTED))
    {
        count++;
        current = current->next;
    }
    array = (char **)malloc((count + 1) * sizeof(char *));
    if (!array)
        return (NULL);
    add_to_grbg(&tool->grbg, array);
    array[i++] = ft_strdup((*input)->value, tool);
    add_to_grbg(&tool->grbg, array[i-1]);
    current = (*input)->next;
    while (i < count && current && (current->type == TOKEN_WORD || current->type == TOKEN_SINGLE_QUOTED_WORD || 
                                     current->type == TOKEN_DOUBLE_QUOTED_WORD ||  current->type == TOKEN_HAS_QUOTED))
    {
        array[i++] = ft_strdup(current->value, tool);
        add_to_grbg(&tool->grbg, array[i-1]);
        current = current->next;
    }
    array[i] = NULL;
    *input = current;
    return (array);
}

t_tree *command_unit2(t_token **input, t_tool *tool, t_redir *before)
{
    t_tree *node;

    node = create_tree_node(NODE_COMMAND, tool);
    if (!node)
        return (NULL);
    node->redirs_before = before;
    node->cmd = create_cmd_array(input, tool);
    if (!node->cmd)
    {
        free(node); // Clean up if cmd creation fails
        return (NULL);
    }
    node->redirs_after = redir(input, tool);
    return (node);
}

t_tree *command_unit(t_token **input, t_tool *tool)
{
    t_redir *before;
    t_tree *node;

    before = redir(input, tool);
    if (!*input || (*input)->type != TOKEN_WORD)
    {
        // Create a command node with just before redirections (no command)
        node = create_tree_node(NODE_COMMAND, tool);
        if (!node)
            return (NULL);
        node->redirs_before = before;
        node->redirs_after = NULL;
        return (node);
    } 
    return (command_unit2(input, tool, before));
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
	//print_token_list(token);

    if (pars_err(&token, tool))
        return (NULL);

    puts("\n");
    t_tree *result = command_unit(&token, tool);
    print_tree(result, 1);
	return (NULL);
}







// t_tree *command_unit(t_token **input, t_tool *tool)
// {
//     t_redir *before;
//     t_redir *after;
//     t_tree *node;
//     t_token *tmp;

//     // Initialize variables
//     before = NULL;
//     after = NULL;
//     node = NULL;
//     tmp = *input;

//     // Handle parenthesized expression
//     // if (*input && (*input)->type == TOKEN_PAREN_OPEN)
//     // {
//     //     *input = (*input)->next; // Skip '('
//     //     node = ft_tree(input, tool); // Parse subtree
//     //     if (!node)
//     //         return (NULL);
//     //     // Optionally parse redirections after closing parenthesis
//     //     after = redir(input, tool);
//     //     node->redirs = after; // Attach redirections to subtree 
//     //     return (node);
//     // }

//     // Process redirections before the command
//     before = redir(input, tool);

//     // If no input or not a WORD, create an empty node with redirections
//     if (!*input || (*input)->type != TOKEN_WORD)
//     {
//         node = create_tree_node(NODE_COMMAND, tool);
//         if (!node)
//             return (NULL);
//         node->redirs = before;
//         return (node);
//     }

//     // Process valid command with command_unit2
//     return (command_unit2(input, tool, before));
// }



// t_token *command_unit2(t_token **input, t_tool *tool, t_token *left_token, t_redir *before)
// {
//     t_redir *after;
//     t_tree *left;
//     left = create_tree_node(NODE_COMMAND, tool);
//     left_token->before = before; 
//     left->cmd = create_cmd_array(input, tool);
//     //*input = (*input)->next;
//     after = redir(input, tool);
//     left_token->after = after;
//     return (left_token);
// }
// t_token *command_unit(t_token **input, t_tool *tool)
// {
//     t_redir *before;
//     t_redir *after;
//     t_token *left;
//     t_token *tmp;
//     before = NULL;
//     after = NULL;
//     left = NULL;
//     tmp = *input;
//     before = redir(input, tool);
//     if (!*input || (*input)->type != TOKEN_WORD)
//     {
//         left = create_tree_node(NODE_COMMAND, tool);
//         if (!left)
//             return (NULL);
//         left->before = before;
// 		left->after = after;
//         return (left);
//     }
//     return (command_unit2(input, tool, tmp, before));
// }


// t_redir *redir(t_token **input, t_tool *tool)
// {
//     t_redir	*redir;
//     size_t redir_index;

//     redir = NULL;
//     redir_index = 0;
//     printf("DEBUG: Entering redir, input token: %s (type: %d)\n",
//            *input ? (*input)->value : "NULL", *input ? (*input)->type : -1);
//     while (*input && ((*input)->type == TOKEN_REDIR_IN || (*input)->type == TOKEN_REDIR_OUT ||
//             (*input)->type == TOKEN_REDIR_APPEND || (*input)->type == TOKEN_REDIR_HEREDOC))
//     {
//         printf("DEBUG: Processing redirection token: %s (type: %d), index: %zu\n",
//                (*input)->value, (*input)->type, redir_index);
//         add_redir(&redir, create_redir(*input, tool, redir_index));
//         *input = (*input)->next;
//         redir_index++;
//     }
//     printf("DEBUG: Exiting redir, redir list created\n");
//     return (redir);
// }


// void print_tree(t_tree *node, int depth)
// {
//     if (!node)
//     {
//         printf("%*s[NULL node]\n", depth * 2, "");
//         return;
//     }

//     const char *indent = "  ";
//     for (int i = 0; i < depth; ++i)
//         printf("%s", indent);

//     printf("Node Type: %s\n", node_type_to_str(node->type));

//     // Print command if it's a command node
//     if (node->type == NODE_COMMAND)
//     {
//         for (int i = 0; node->cmd && node->cmd[i]; ++i)
//         {
//             for (int j = 0; j < depth; ++j)
//                 printf("%s", indent);
//             printf("  cmd[%d] = %s\n", i, node->cmd[i]);
//         }
//     }

//     // Print redirections
//     for (int i = 0; i < depth; ++i)
//         printf("%s", indent);
//     printf("  Redirections:\n");
//     print_redir(node->redirs);

//     // // Print ambiguity
//     // for (int i = 0; i < depth; ++i)
//     //     printf("%s", indent);
//     // printf("  Is Ambiguous: %d\n", node->is_ambiguous);

//     // // Recurse into left and right nodes
//     // for (int i = 0; i < depth; ++i)
//     //     printf("%s", indent);
//     // printf("Left:\n");
//     // print_tree(node->left, depth + 1);

//     // for (int i = 0; i < depth; ++i)
//     //     printf("%s", indent);
//     // printf("Right:\n");
//     // print_tree(node->right, depth + 1);
// }



// t_tree *command_unit2(t_token **input, t_tool *tool, t_redir *before)
// {
//     t_tree *node;
//     t_redir *after;

//     // Create command node
//     node = create_tree_node(NODE_COMMAND, tool);
//     if (!node)
//         return (NULL);
    
//     // Assign the "before" redirections that were parsed earlier
//     node->redirs = before;
    
//     // Create command array (command name + arguments)
//     node->cmd = create_cmd_array(input, tool);
    
//     // Get any redirections that follow the command
//     after = redir(input, tool);
    
//     // Append the "after" redirections to the redirection list
//     if (after)
//     {
//         // If we already have redirections, append to the end
//         if (node->redirs)
//             add_redir(&node->redirs, after);
//         else
//             node->redirs = after;
//     }

//     return (node);
// }

// // Main command unit parsing function
// t_tree *command_unit(t_token **input, t_tool *tool)
// {
//     t_redir *before;
//     t_tree *node;

//     // Parse any redirections that come before the command
//     before = redir(input, tool);

//     // If we've reached the end of input or the current token isn't a command word
//     if (!*input || (*input)->type != TOKEN_WORD)
//     {
//         // Create a command node with just redirections (no command)
//         node = create_tree_node(NODE_COMMAND, tool);
//         if (!node)
//             return (NULL);
//         node->redirs = before;
//         return (node);
//     }

//     // Process the command and any redirections that follow it
//     return (command_unit2(input, tool, before));
// }