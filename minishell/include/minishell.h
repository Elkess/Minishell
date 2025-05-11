/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 11:49:41 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/11 11:00:00 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
#include <stdbool.h>
# include <fcntl.h>

typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_SPACE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_REDIR_HEREDOC,
    TOKEN_PAREN_OPEN,
    TOKEN_PAREN_CLOSE,
    TOKEN_SINGL_AND = 11,
    TOKEN_SINGLE_QUOTE = 12, // represent the quote characters as delimiters.
    TOKEN_DOUBLE_QUOTE = 13,
    TOKEN_DOLLAR = 14,
    TOKEN_WILDCARD = 15,
    TOKEN_SINGLE_QUOTED_WORD = 16, // Single-quoted string, e.g., 'hello'
    TOKEN_DOUBLE_QUOTED_WORD = 17, // Double-quoted string, e.g., "world $USER"
    TOKEN_HAS_QUOTED = 18, // Has quotes
    TOKEN_FILERED_OUT = -2,   // File after >
    TOKEN_FILERED_IN = -3,    // File after <
    TOKEN_FILERED_APPEND = -5,// File after >>
    TOKEN_FILERED_HEREDOC = -4 // Delimiter after <<
} t_token_type;

typedef struct s_token
{
    char           *value;
    int				priority;
    t_token_type    type;
    struct s_token *next;
} t_token;

/**************************************************/

typedef struct s_garbcoll
{
	void				*ptr;
	struct s_garbcoll	*next;
}	t_garbcoll;

typedef struct s_tool
{
    int         paren; // 0 for null , 1 for ()
	int			quoted; // 0 for null , 1 for singl , 2 for duble 
	int			anderr; // is singl &
    int			err;
    t_garbcoll	*grbg; // for garbage collect
}	t_tool;

typedef enum e_priority
{
	prio_pipe = 1,
	prio_and = 2,
	prio_or = 3,
	prio_redir_in = 4,
	prio_redir_out = 5,
	prio_appand = 6,
	prio_here_doc = 7,
	prio_open_par = 8,
	prio_close_par = 9,
}   t_priority;

/**************************************************/

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
    REDIR_HEREDOC,    // <<
    REDIR_NONE       // Invalid or no redirection
}   t_redir_type;

typedef struct s_redir
{
    size_t            index;
    t_redir_type      type;
    char             *file;
    int               fd;
    int               flag;
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

// Function declarations

t_tree	*parsing_input(char *line, t_tool *tool);
t_token	*tokens_lst(char *cmd, t_tool *tool);
t_token	*check_token(t_token **token, t_tool *tool);

t_token		*lst_new(void *str, t_tool *tool);
t_token     *lastone(t_token *head);
void		lst_add_back(t_token **head, t_token *token);
void        hundel_quotes_paren(t_tool *tool, char cmd);
int         is_delimter(char c, char d);
void        init_redir_file_tokens(t_token *token);

void        add_to_grbg(t_garbcoll **head, void *value);
void        clear_garbcoll(t_garbcoll *head);

size_t	ft_strlen(const char *s);
int     is_space(char c);
char	*ft_strdup(const char *s1, t_tool *tool);
char	*ft_strchr(const char *s, int c);
int     ft_strcmp(char *s1, char *s2);
char	*ft_my_strdup(const char *s1, size_t size, t_tool *tool);

void	init_token(t_token **token, int priority, int type);
int	pars_err(t_token **token, t_tool *tool);



#endif