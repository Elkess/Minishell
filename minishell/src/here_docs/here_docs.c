/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_docs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:17:26 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/01 20:30:26 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_herdocs(t_tree *tree, t_env *envh, t_tool *tool)
{
	if (!tree)
		return ;
	if (tool->herdoc_err == 1)
		return ;
	here_docs(tree->redirs, envh, tool);
	handle_herdocs(tree->left, envh, tool);
	handle_herdocs(tree->right, envh, tool);
}
