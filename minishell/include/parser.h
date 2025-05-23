/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:32:27 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/23 08:34:06 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdlib.h>

typedef struct s_tree		t_tree;
typedef struct s_redir		t_redir;
typedef enum e_node_type	t_node_type;

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
	TOKEN_SINGLE_QUOTE = 12,
	TOKEN_DOUBLE_QUOTE = 13,
	TOKEN_DOLLAR = 14,
	TOKEN_WILDCARD = 15,
	TOKEN_SINGLE_QUOTED_WORD = 16,
	TOKEN_DOUBLE_QUOTED_WORD = 17,
	TOKEN_HAS_QUOTED = 18,
	TOKEN_FILERED_OUT = -2,
	TOKEN_FILERED_IN = -3,
	TOKEN_FILERED_APPEND = -5,
	TOKEN_FILERED_HEREDOC = -4
}	t_token_type;

typedef struct s_token
{
	char			*value;
	int				priority;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_garbcoll
{
	void				*ptr;
	struct s_garbcoll	*next;
}	t_garbcoll;

typedef struct s_tool
{
	int			paren;
	int			quoted;
	int			anderr;
	int			err;
	t_garbcoll	*grbg;
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
}	t_priority;

// Parser functions
t_tree	*parsing_input(char *line, t_tool *tool);
t_token	*tokens_lst(char *cmd, t_tool *tool);
void	init_quote_token(t_token *token);
t_token	*update_token(t_token **token);
int		pars_err_utils(t_token *token, t_tool *tool);
t_token	*lst_new(void *str, t_tool *tool);
t_token	*lastone(t_token *head);
void	lst_add_back(t_token **head, t_token *token);
void	hundel_quotes_paren(t_tool *tool, char cmd);
int		is_delimter(char c, char d);
void	init_redir_file_tokens(t_token *token);
void	add_to_grbg(t_garbcoll **head, void *value);
void	clear_garbcoll(t_garbcoll *head);
int		pars_err(t_token **token, t_tool *tool);
t_tree	*node_command(t_token **input, t_tool *tool);
t_tree	*node_command_utils(t_token **input, t_tool *tool, t_redir *before);
t_tree	*ft_tree(t_token **control, t_tool *tool);
t_redir	*create_redir(t_token *token, t_tool *tool, size_t index);

t_redir	*last_node(t_redir *redirs);
void	add_redir(t_redir **redirs, t_redir *new_redir);
t_redir	*redir(t_token **input, t_tool *tool);
t_tree	*create_tree_node(t_node_type type, t_tool *tool);
t_redir	*concat_redirs(t_redir *before, t_redir *after, t_tool *tool);

// Parser-specific libft functions
size_t	ft_strlen(const char *s);
int		is_space(char c);
char	*ft_strdup_pars(const char *s1, t_tool *tool);
char	*ft_strchr(const char *s, int c);
char	*ft_my_strdup(const char *s1, size_t size, t_tool *tool);
void	init_token(t_token **token, int priority, int type);
char	**create_cmd_array(t_token **input, t_tool *tool);

#endif