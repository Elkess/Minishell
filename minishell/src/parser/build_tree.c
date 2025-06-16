/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 09:20:22 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/16 11:47:52 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_redir	*redir(t_token **input, t_tool *tool)
{
	t_redir	*redir;
	size_t	redir_index;
	t_token *redir_target;
	t_token	*current;
	t_token	*prev;

	redir = NULL;
	redir_index = 0;
	current = *input;
	prev = NULL;

	while (current)
	{
		if (current->type == 1 || current->type == 2 || current->type == 3 || current->type == 9 || current->type == 10)
			break;
		if (current->type >= 5 && current->type <= 8)
		{
			redir_target = current->next;
			if (redir_target)
			{
				add_redir(&redir, create_redir(current, tool, redir_index++));
				if (prev)
					prev->next = redir_target;
				else
					*input = redir_target;

				current = redir_target->next;
				if (prev)
					prev->next = current;
				else
					*input = current;
			}
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
	return (redir);
}

t_tree	*node_command_utils(t_token **input, t_tool *tool, t_redir *before)
{
	t_tree	*node;

	node = create_tree_node(0, tool);
	if (!node)
		return (NULL);
	node->redirs_before = before;
	node->cmd = create_cmd_array(input, tool);
	if (!node->cmd)
		return (NULL);
	node->redirs_after = redir(input, tool);
	node->redirs = concat_redirs(node->redirs_before, node->redirs_after, tool);
	return (node);
}

t_tree	*node_command(t_token **input, t_tool *tool)
{
	t_redir	*before;
	t_tree	*node;

	before = redir(input, tool);
	if (*input && (*input)->type == 9)
	{
		*input = (*input)->next;
		node = create_tree_node(4, tool);
		node->redirs_before = before;
		node->left = ft_tree(input, tool);
		if (*input && (*input)->type == 10)
			*input = (*input)->next;
		node->redirs_after = redir(input, tool);
		node->redirs = concat_redirs(node->redirs_before,
				node->redirs_after, tool);
		return (node);
	}
	if (!*input || ((*input)->type != 17 && (*input)->type != 18
			&& (*input)->type != 16 && (*input)->type != 0))
	{
		node = create_tree_node(0, tool);
		node->redirs_before = before;
		node->redirs_after = NULL;
		node->redirs = concat_redirs(node->redirs_before,
				node->redirs_after, tool);
		return (node);
	}
	return (node_command_utils(input, tool, before));
}

t_tree	*node_pipe(t_token **input, t_tool *tool)
{
	t_tree	*left;
	t_tree	*right;
	t_tree	*pipe;

	pipe = NULL;
	left = node_command(input, tool);
	while (*input && (*input)->type == 1)
	{
		pipe = create_tree_node(1, tool);
		*input = (*input)->next;
		right = node_pipe(input, tool);
		pipe->left = left;
		pipe->right = right;
		left = pipe;
	}
	return (left);
}

t_tree	*ft_tree(t_token **control, t_tool *tool)
{
	t_tree		*left;
	t_tree		*right;
	t_tree		*op;
	t_node_type	node_type;

	op = NULL;
	left = node_pipe(control, tool);
	while (*control && ((*control)->type == 2 || (*control)->type == 3))
	{
		if ((*control)->type == 2)
			node_type = 2;
		else
			node_type = 3;
		op = create_tree_node(node_type, tool);
		*control = (*control)->next;
		right = node_pipe(control, tool);
		op->right = right;
		op->left = left;
		left = op;
	}
	return (left);
}
