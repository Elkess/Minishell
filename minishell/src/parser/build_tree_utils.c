/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 09:22:22 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/18 10:35:53 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_redir *create_redir(t_token *token, t_tool *tool, size_t index)
{
    t_redir	*redir;

    redir = (t_redir *) malloc(sizeof(t_redir));
    if (!redir)
        return (NULL);
    add_to_grbg(&tool->grbg, redir);
    
    redir->index = index;
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
    node->redirs = NULL;
    node->redirs_before = NULL;
    node->redirs_after = NULL;
    node->is_ambiguous = 0;
    node->left = NULL;
    node->right = NULL;
    return (node);
}
void	add_to_index(t_redir *after, int index)
{
	while(after)
	{
		after->index = index +1;
		index++;
		after = after->next;
	}
}

t_redir *concat_redirs(t_redir *before, t_redir *after, t_tool *tool)
{
    t_redir *new_list = NULL;
    t_redir *curr;

	curr = before;
    while (before && before->next)
		before = before->next;
	if (before)
	{
		add_to_index(after, before->index);
		before->next = after;
		new_list = curr;
	}
	else
		new_list = after;
    return (new_list);
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
    array[i++] = ft_strdup_pars((*input)->value, tool);
    add_to_grbg(&tool->grbg, array[i-1]);
    current = (*input)->next;
    while (i < count && current && (current->type == TOKEN_WORD || current->type == TOKEN_SINGLE_QUOTED_WORD || 
                                     current->type == TOKEN_DOUBLE_QUOTED_WORD ||  current->type == TOKEN_HAS_QUOTED))
    {
        array[i++] = ft_strdup_pars(current->value, tool);
        add_to_grbg(&tool->grbg, array[i-1]);
        current = current->next;
    }
    array[i] = NULL;
    *input = current;
    return (array);
}
