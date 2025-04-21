/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 11:49:41 by sgmih             #+#    #+#             */
/*   Updated: 2025/04/21 14:04:32 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
#include <stdbool.h>

# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
#include <string.h>
#include <dirent.h>
# include <fcntl.h>


typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_REDIR_HEREDOC,
    TOKEN_PAREN_OPEN,
    TOKEN_PAREN_CLOSE 
    ///TOKEN_GROUP
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

typedef struct	s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;


// function libft
int         ft_atoi(const char *str);
long long   ft_atol(const char *str,int sign);
size_t      ft_strlen(const char *s);
size_t      ft_dstrlen(const char **s);
char        **ft_split(char const *s, char c);
char        *ft_strjoin(char const *s1, char const *s2);
void        ft_putstr_fd(char *s, int fd);
void        ft_putchar_fd(char c, int fd);
char        *ft_substr(char const *s, unsigned int start, size_t len);
char        *ft_strdup(const char *s1);
int         ft_strcmp(const char *s1, const char *s2);
int         ft_strncmp(const char *s1, const char *s2, size_t n);
int         ft_isalpha(int a);
int         ft_isdigit(int n);
int         ft_isalnum(int c);
char        *ft_strchr(const char *s, int c);


#endif