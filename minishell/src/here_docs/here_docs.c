/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_docs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:17:26 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/22 18:17:43 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_herdocs(t_tree *tree, t_env *envh)
{
	if (!tree)
		return ;
	here_docs(tree->redirs, envh);
	handle_herdocs(tree->left, envh);
	handle_herdocs(tree->right, envh);
}
