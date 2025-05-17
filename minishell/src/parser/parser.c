/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:35:16 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/17 14:22:29 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"

/******************************************** start print tokens *********************************************/


// Get node type string
const char *get_node_type_str(t_node_type type) {
    switch (type) {
        case NODE_COMMAND: return "CMD";
        case NODE_PIPE: return "PIPE";
        case NODE_OR: return "||";
        case NODE_AND: return "&&";
        default: return "UNKNOWN";
    }
}

// Get redirection type string
const char *get_redir_type_str(t_redir_type type) {
    switch (type) {
        case REDIR_IN: return "< (REDIR_IN)";
        case REDIR_OUT: return "> (REDIR_OUT)";
        case REDIR_APPEND: return ">> (REDIR_APPEND)";
        case REDIR_HEREDOC: return "<< (REDIR_HEREDOC)";
        default: return "Unknown (REDIR_NONE)";
    }
}

// Print command and redirection details for a node
void print_node_details(t_tree *node, char *prefix, int is_last, int level) {
    if (node->type == NODE_COMMAND) {
        // Print command node header
        printf("%s%s Command Node: %u\n", prefix, is_last ? "└──" : "├──", node->type);

        // Print command
        printf("%s%s  Cmd: ", prefix, is_last ? "    " : "│   ");
        if (node->cmd && node->cmd[0]) {
            for (int i = 0; node->cmd[i]; i++)
                printf("%s ", node->cmd[i]);
            printf("\n");
        } else {
            printf("(none)\n");
        }

        // Print redirections before
        printf("%s%s  Redirs Before: ", prefix, is_last ? "    " : "│   ");
        if (node->redirs_before) {
            printf("\n");
            t_redir *curr = node->redirs_before;
            while (curr) {
                printf("%s%s    Index     : %zu\n", prefix, is_last ? "    " : "│   ", curr->index);
                printf("%s%s    Type      : %s\n", prefix, is_last ? "    " : "│   ", get_redir_type_str(curr->type));
                printf("%s%s    File      : %s\n", prefix, is_last ? "    " : "│   ", curr->file ? curr->file : "(null)");
                printf("%s%s    FD        : %d\n", prefix, is_last ? "    " : "│   ", curr->fd);
                curr = curr->next;
            }
        } else {
            printf("(none)\n");
        }

        // Print redirections after
        printf("%s%s  Redirs After: ", prefix, is_last ? "    " : "│   ");
        if (node->redirs_after) {
            printf("\n");
            t_redir *curr = node->redirs_after;
            while (curr) {
                printf("%s%s    Index     : %zu\n", prefix, is_last ? "    " : "│   ", curr->index);
                printf("%s%s    Type      : %s\n", prefix, is_last ? "    " : "│   ", get_redir_type_str(curr->type));
                printf("%s%s    File      : %s\n", prefix, is_last ? "    " : "│   ", curr->file ? curr->file : "(null)");
                printf("%s%s    FD        : %d\n", prefix, is_last ? "    " : "│   ", curr->fd);
                curr = curr->next;
            }
        } else {
            printf("(none)\n");
        }
    } else {
        // Print operator node
        printf("%s%s Operator Node: %s\n", prefix, is_last ? "└──" : "├──", get_node_type_str(node->type));
    }
}

// Get annotation for a node
void get_node_annotation(t_tree *node, t_tree *parent, int is_left, char *buffer, size_t buf_size) {
    char node_str[256];
    snprintf(node_str, sizeof(node_str), "%s", node->type == NODE_COMMAND ? "CMD" : get_node_type_str(node->type));
    
    if (!parent) {
        snprintf(buffer, buf_size, "// Root");
    } else {
        char parent_str[256];
        snprintf(parent_str, sizeof(parent_str), "%s", parent->type == NODE_COMMAND ? "CMD" : get_node_type_str(parent->type));
        snprintf(buffer, buf_size, "// %s subtree of %s", is_left ? "Left" : "Right", parent_str);
    }
}

// Recursive function to print tree
void print_tree_recursive(t_tree *node, char *prefix, int is_last, t_tree *parent, int is_left, int level) {
    if (!node) return;

    // Print node details
    print_node_details(node, prefix, is_last, level);

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
        printf("Empty tree\n");
        return;
    }
    print_tree_recursive(node, "", 1, NULL, 0, 0);
}

// Helper to create a command node
t_tree *create_command_node(char **cmd) {
    t_tree *node = calloc(1, sizeof(t_tree));
    node->type = NODE_COMMAND;
    node->cmd = cmd;
    return node;
}

// Helper to create an operator node
t_tree *create_operator_node(t_node_type type, t_tree *left, t_tree *right) {
    t_tree *node = calloc(1, sizeof(t_tree));
    node->type = type;
    node->left = left;
    node->right = right;
    return node;
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

    if ((*input)->type == TOKEN_PAREN_OPEN)
    {
        *input = (*input)->next; // Skip '('
        node = ft_tree(input, tool);
        if (!node)
            return (NULL);
        *input = (*input)->next; // Skip ')'
        node->redirs_after = redir(input, tool); // Handle any redirections after
        return (node);
    }
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

t_tree	*pipe_unit(t_token **input, t_tool *tool)
{
    t_tree	*left;
	t_tree	*right;
	t_tree	*pipe;

    pipe = NULL;
	left = command_unit(input, tool);

    while (*input && (*input)->type == TOKEN_PIPE)
    {
        pipe = create_tree_node(NODE_PIPE, tool);
        *input = (*input)->next;
		right = pipe_unit(input, tool);
        pipe->left = left;
		pipe->right = right;
        left = pipe;
    }
    return (left);
}

t_tree	*ft_tree(t_token **control, t_tool *tool)
{
    t_tree	*left;
	t_tree	*right;
	t_tree	*op;

    op = NULL;
	left = pipe_unit(control, tool);
    	while (*control && ((*control)->type == TOKEN_AND || (*control)->type == TOKEN_OR))
	{
        t_node_type node_type;
        if ((*control)->type == TOKEN_AND)
            node_type = NODE_AND;
        else
            node_type = NODE_OR;
        op = create_tree_node(node_type, tool);
		*control = (*control)->next;
		right = pipe_unit(control, tool);
		op->right = right;
		op->left = left;
		left = op;
	}
	return (left);
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

    t_tree *result = ft_tree(&token, tool);
    print_tree(result);

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