/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:32:27 by sgmih             #+#    #+#             */
/*   Updated: 2025/04/20 08:49:57 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../include/minishell.h"
# include <unistd.h>
# include <stdlib.h>

t_token *tokenize_input(const char *line);

#endif