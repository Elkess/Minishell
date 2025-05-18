/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:35:16 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/18 09:29:56 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/******************************************** start print tokens *********************************************/


// #include <stdio.h>

// void print_redir_list(t_redir *redir, const char *list_name, int indent)
// {
//     t_redir *curr = redir;
//     int i;

//     for (i = 0; i < indent; i++)
//         printf("  ");
//     printf("%s:\n", list_name);
//     if (!curr)
//     {
//         for (i = 0; i < indent + 1; i++)
//             printf("  ");
//         printf("(none)\n");
//         return;
//     }
//     while (curr)
//     {
//         for (i = 0; i < indent + 1; i++)
//             printf("  ");
//         printf("type: ");
//         switch (curr->type)
//         {
//             case REDIR_IN: printf("REDIR_IN"); break;
//             case REDIR_OUT: printf("REDIR_OUT"); break;
//             case REDIR_APPEND: printf("REDIR_APPEND"); break;
//             case REDIR_HEREDOC: printf("REDIR_HEREDOC"); break;
//             case REDIR_NONE: printf("REDIR_NONE"); break;
//             default: printf("UNKNOWN"); break;
//         }
//         printf(", file: %s, index: %zu\n", curr->file ? curr->file : "(null)", curr->index);
//         curr = curr->next;
//     }
// }

// void print_tree_redirs(t_tree *tree, int indent)
// {
//     int i;

//     if (!tree)
//         return;

//     // Print node info
//     for (i = 0; i < indent; i++)
//         printf("  ");
//     printf("Node type: ");
//     switch (tree->type)
//     {
//         case NODE_COMMAND: printf("COMMAND"); break;
//         case NODE_PIPE: printf("PIPE"); break;
//         case NODE_AND: printf("AND"); break;
//         case NODE_OR: printf("OR"); break;
//         case NODE_PARENTHS: printf("PARENTHS"); break;
//         default: printf("UNKNOWN"); break;
//     }
//     printf("\n");

//     // Print redirections
//     print_redir_list(tree->redirs, "redirs", indent + 1);
//     print_redir_list(tree->redirs_before, "redirs_before", indent + 1);
//     print_redir_list(tree->redirs_after, "redirs_after", indent + 1);

//     // Recurse on children
//     print_tree_redirs(tree->left, indent + 1);
//     print_tree_redirs(tree->right, indent + 1);
// }

/******************************************** end print tokens *********************************************/

t_tree	*parsing_input(char *line, t_tool *tool)
{
	t_token	*token;
    t_tree	*tree;
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

    tree = ft_tree(&token, tool);
    print_tree_redirs(tree, 0);
    //print_tree(tree); 

	return (tree);
}
