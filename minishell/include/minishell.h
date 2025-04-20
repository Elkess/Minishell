/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 11:49:41 by sgmih             #+#    #+#             */
/*   Updated: 2025/04/19 12:02:08 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>

typedef enum e_token_type
{
    TOKEN_WORD  = 1,
    TOKEN_PIPE   = 2,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_REDIR_HEREDOC,
    TOKEN_GROUP
} t_token_type;

typedef struct s_token
{
    t_token_type    type;
    char           *value;
    struct s_token *next;
} t_token;

typedef enum e_node_type
{
    NODE_COMMAND,
    NODE_PIPE,
    NODE_AND,
    NODE_OR
}   t_node_type;

typedef enum e_redir_type
{
    REDIR_IN,        // <
    REDIR_OUT,       // >
    REDIR_APPEND,    // >>
    REDIR_HEREDOC    // <<
}   t_redir_type;

typedef struct s_redir
{
    t_redir_type      type;
    char             *file;
    struct s_redir   *next;
}   t_redir;

typedef struct s_tree
{
    t_node_type      type;
    char            **cmd;        // argv if NODE_COMMAND
    t_redir          *redirs;     // linked list of redirs
    int              fd[2];       // pipe fds (used during execution)
    int              is_ambiguous;

    struct s_tree    *left;       // left command (for pipe)
    struct s_tree    *right;      // right command (for pipe)
}   t_tree;


t_token *tokenize_input(const char *line);

#endif