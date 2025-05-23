/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 09:20:22 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/23 08:18:19 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_redir	*redir(t_token **input, t_tool *tool)
{
	t_redir	*redir;
	size_t	redir_index;

	redir = NULL;
	redir_index = 0;
	while (*input && ((*input)->type == 5 || (*input)->type == 6
			|| (*input)->type == 7 || (*input)->type == 8))
	{
		add_redir(&redir, create_redir(*input, tool, redir_index));
		*input = (*input)->next;
		if (*input)
			*input = (*input)->next;
		redir_index++;
	}
	return (redir);
}

t_tree	*node_command_utils(t_token **input, t_tool *tool, t_redir *before)
{
	t_tree	*node;

	node = create_tree_node(NODE_COMMAND, tool);
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
	if (!*input || (*input)->type != 0)
	{
		node = create_tree_node(0, tool);
		if (!node)
			return (NULL);
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
	while (*input && (*input)->type == TOKEN_PIPE)
	{
		pipe = create_tree_node(NODE_PIPE, tool);
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
		if ((*control)->type == TOKEN_AND)
			node_type = NODE_AND;
		else
			node_type = NODE_OR;
		op = create_tree_node(node_type, tool);
		*control = (*control)->next;
		right = node_pipe(control, tool);
		op->right = right;
		op->left = left;
		left = op;
	}
	return (left);
}
