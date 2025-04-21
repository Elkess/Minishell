/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:32:27 by sgmih             #+#    #+#             */
/*   Updated: 2025/04/21 14:25:11 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../include/minishell.h"

// Function declarations
t_token *parse_input(const char *line);
char    *extract_word(const char *line, int *i);
char	*extract_quoted_string(const char *line, int *i, char quote_char);
int     contains_wildcard(const char *word);
void    free_tokens(t_token *head);
void    add_token(t_token **head, t_token *new_token);
t_token *create_token(t_token_type type, char *value); // Ensure this is here


#endif