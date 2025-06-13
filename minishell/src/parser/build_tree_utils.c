/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 09:22:22 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/11 13:50:58 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_redir	*create_redir(t_token *token, t_tool *tool, size_t index)
{
	t_redir	*redir;

	redir = (t_redir *) malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	add_to_grbg(&tool->grbg, redir);
	redir->index = index;
	if (token->type == 5)
		redir->type = 0;
	else if (token->type == 6)
		redir->type = 1;
	else if (token->type == 7)
		redir->type = 2;
	else if (token->type == 8)
		redir->type = 3;
	else
		redir->type = 4;
	redir->file = token->next->value;
	redir->fd = -1;
	redir->flag = 0;
	redir->is_ambiguous = 0;
	redir->next = NULL;
	return (redir);
}

t_tree	*create_tree_node(t_node_type type, t_tool *tool)
{
	t_tree	*node;

	node = (t_tree *) malloc(sizeof(t_tree));
	if (!node)
		return (NULL);
	add_to_grbg(&tool->grbg, node);
	node->type = type;
	node->cmd = NULL;
	node->redirs = NULL;
	node->redirs_before = NULL;
	node->redirs_after = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

void	add_to_index(t_redir *after, int index)
{
	while (after)
	{
		after->index = index +1;
		index++;
		after = after->next;
	}
}

t_redir	*concat_redirs(t_redir *before, t_redir *after, t_tool *tool)
{
	t_redir	*new_list;
	t_redir	*curr;

	new_list = NULL;
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

char	**create_cmd_array(t_token **input, t_tool *tool)
{
	t_token	*current;
	int		count;
	char	**array;
	int		i;

	current = (*input)->next;
	count = 1;
	i = 0;
	while (current && (current->type != 1 && current->type != 2 && current->type != 3))
	{
		if (current->type == 0 || current->type == 16
			|| current->type == 17 || current->type == 18)
			count++;
		current = current->next;
	}
	array = (char **)malloc((count + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	add_to_grbg(&tool->grbg, array);
	array[i++] = ft_strdup_pars((*input)->value, tool);
	current = (*input)->next;
	while (i < count && current && (current->type != 1 && current->type != 2 && current->type != 3))
	{
		if (current->type == 0 || current->type == 16
			|| current->type == 17 || current->type == 18)
			array[i++] = ft_strdup_pars(current->value, tool);
		current = current->next;
	}
	array[i] = NULL;
	*input = current;
	return (array);
}
