/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 11:49:41 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/18 08:33:37 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
// #include <dirent.h>
#include <sys/stat.h>
#include <errno.h>


typedef enum e_redir_type
{
    REDIR_IN,        // <
    REDIR_OUT,       // >
    REDIR_APPEND,    // >>
    REDIR_HEREDOC,   // <<
    REDIR_NONE       // Invalid or no redirection
} t_redir_type;

typedef struct s_redir
{
    size_t            index;
    t_redir_type      type;
    char             *file;
    int               fd;
    int               flag;
    struct s_redir   *next;
} t_redir;

typedef enum e_node_type
{
    NODE_COMMAND,
    NODE_PIPE,
    NODE_AND,
    NODE_OR,
    NODE_PARENTHS
} t_node_type;

typedef struct s_tree
{
    t_node_type      type;
    char            **cmd;        // argv if NODE_COMMAND
    t_redir          *redirs;    // linked list of redirs
    t_redir          *redirs_before; // Redirections before the command
    t_redir          *redirs_after;  // Redirections after the command
    int              is_ambiguous;
    struct s_tree    *left;       // left command (for pipe)
    struct s_tree    *right;      // right command (for pipe)
} t_tree;

/* Libft functions used both us */
size_t  ft_strlen(const char *s);
void    ft_putstr_fd(char *s, int fd);
char    *ft_strchr(const char *s, int c);
int     ft_strcmp(const char *s1, const char *s2);


#include "executor.h"
#include "parser.h"

#endif