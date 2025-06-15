/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 09:22:22 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/15 11:23:00 by sgmih            ###   ########.fr       */
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
	node->left = NULL;
	node->right = NULL;
	return (node);
}

static char	**cmd_array_utils(t_token **input, int *count)
{
	t_token	*temp;

	*count = 0;
	temp = *input;
	while (temp && temp->type != 1 && temp->type != 2 && temp->type != 3)
	{
		if (temp->type >= 5 && temp->type <= 8)
		{
			if (temp->next)
				temp = temp->next->next;
			else
				break ;
		}
		else
		{
			if (temp->type == 0 || temp->type == 16
				|| temp->type == 17 || temp->type == 18)
				(*count)++;
			temp = temp->next;
		}
	}
	return (NULL);
}

static t_token	*process_token(t_token *current,
	char **array, int *i, t_tool *tool)
{
	if (current->type >= 5 && current->type <= 8)
	{
		if (current->next)
			return (current->next->next);
		return (NULL);
	}
	else if (current->type == 0 || current->type == 16
		|| current->type == 17 || current->type == 18)
		array[(*i)++] = ft_strdup_pars(current->value, tool);
	return (current->next);
}

char	**create_cmd_array(t_token **input, t_tool *tool)
{
	t_token	*current;
	char	**array;
	int		count;
	int		i;

	cmd_array_utils(input, &count);
	array = (char **)malloc((count + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	add_to_grbg(&tool->grbg, array);
	current = *input;
	i = 0;
	while (i < count && current && current->type != 1
		&& current->type != 2 && current->type != 3)
	{
		current = process_token(current, array, &i, tool);
		if (!current)
			break ;
	}
	array[i] = NULL;
	*input = current;
	return (array);
}
