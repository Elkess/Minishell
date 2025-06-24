/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 11:49:41 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/24 11:09:08 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <fcntl.h>
# include <limits.h>
# include <string.h>
# include <sys/stat.h>
# include <errno.h>
# include <termios.h> 
# include <dirent.h>

int	g_signal;

# define BUFFER_SIZE 1

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

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_pid
{
	int				value;
	struct s_pid	*next;
}	t_pid;

typedef struct s_tool
{
	int				paren;
	int				quoted;
	int				anderr;
	int				err;
	int				herdoc_err;
	int				signal;
	int				flag;
	int				inside_pipe;
	int				status;
	pid_t			pid;
	char			**path;
	char			*tmp;
	char			*pwd_backup;
	t_env			*envh;
	t_garbcoll		*grbg;
	int				status_exp;
	int				home;
	char			*new_arg;
	char			*new_path;
	size_t			dot_counter;
	struct termios	orig_termios;
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

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
	REDIR_NONE
}	t_redir_type;

typedef struct s_redir
{
	size_t			index;
	t_redir_type	type;
	char			*file;
	char			*content;
	int				fd;
	int				flag;
	int				is_ambiguous;
	struct s_redir	*next;
}	t_redir;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_PARENTHS
}	t_node_type;

typedef struct s_expand
{
	int		flg;
	int		i;
	int		j;
	int		is_wildcard;
	int		before_qoute;
	int		is_char;
	int		is_there_export;
	int		var_not_found;
	char	*buff_exp;
	t_token	*token;
}	t_expand;

typedef struct s_redir_context
{
	t_token	*cur;
	t_token	*prev;
	size_t	indx;
}	t_redir_context;

typedef struct s_tree
{
	t_node_type		type;
	char			**cmd;
	t_redir			*redirs;
	t_redir			*redirs_before;
	t_redir			*redirs_after;
	struct s_tree	*left;
	struct s_tree	*right;
}	t_tree;

char		*get_next_line(int fd);
void		setup_signals(void);
void		disable_echoctl(struct termios *orig_termios);
void		restore_terminal(struct termios *orig_termios);
t_tree		*parsing_input(char *line, t_tool *tool);
t_tree		*ft_tree(t_token **control, t_tool *tool);
t_tree		*create_tree_node(t_node_type type, t_tool *tool);
t_token		*tokens_lst(char *cmd, t_tool *tool);
t_token		*update_token(t_token **token);
t_token		*lst_new(void *str, t_tool *tool);
t_token		*lastone(t_token *head);
t_redir		*create_redir(t_token *token, t_tool *tool, size_t index);
t_redir		*last_node(t_redir *redirs);
t_redir		*redir(t_token **input, t_tool *tool);
void		init_quote_token(t_token *token);
void		init_redir_file_tokens(t_token *token);
void		lst_add_back(t_token **head, t_token *token);
void		hundel_quotes_paren(t_tool *tool, char cmd);
void		add_redir(t_redir **redirs, t_redir *new_redir);
void		add_to_grbg(t_garbcoll **head, void *value);
void		clear_garbcoll(t_garbcoll *head);
int			is_delimter(char c, char d);
int			pars_err(t_token **token, t_tool *tool);
char		*strjoin_char(char *str, char c, t_tool *tool);
char		**handel_expand(t_tree *tree, int exit_status, t_tool *tool);
char		**create_cmd_array_2(t_token *token, t_tool *tool);
int			has_space(const char *str);
int			valid_char(char c);
void		expand_redir(t_tree *tree, t_tool *tool, int status);
t_token		*new_lst(void *content, t_tool *tool);
char		**expand_wildcard(char *buff_exp, t_tool *tool);
int			get_list_match(char *str, char *wildcard, t_tool *tool);
t_redir		*concat_redirs(t_redir *before, t_redir *after);
int			one_wildcard(char *str);
size_t		ft_strlen(const char *s);
int			is_space(char c);
char		*ft_strdup_pars(const char *s1, t_tool *tool);
char		*ft_strchr(const char *s, int c);
char		*ft_my_strdup(const char *s1, size_t size, t_tool *tool);
void		init_token(t_token **token, int priority, int type);
char		**create_cmd_array(t_token **input, t_tool *tool);
size_t		ft_strlen(const char *s);
void		ft_putstr_fd(char *s, int fd);
char		*ft_strchr(const char *s, int c);
int			ft_strcmp(const char *s1, const char *s2);
void		print_err(char *msg1, char *arg, char *msg2);
void		free_envh(t_env *envh);
int			execute_tree(t_tree *tree, t_env **envh, t_tool	*tool);
int			executor(t_tree *tree, t_env **envh, t_tool	*tool);
int			ft_echo(t_tree *cmd1, t_tool *tool);
int			pwd(char *cd_path);
int			env(t_env *envh, t_tool *tool);
int			cd(t_env **envh, t_tree *cmd, char **fake_pwd, t_tool *tool);
int			ft_export(t_env **envh, t_tree *tree, t_tool *tool);
int			unset(t_env **envh, char **args);
int			ft_exit(t_tree *cmd, int status, t_env *envh, t_tool *tool);
t_env		*fill_env(char **envp);
char		*ext_data(char *line, int flag);
t_env		*append_node(t_env *head, char *key, char *val);
t_env		*sh_env(t_env *envh, char *key);
int			is_valid_key(char *key);
t_env		*edit_env(char *key, char *val, t_env *envh, int should_concat);
void		print_rest(char **s, int n, t_tool *tool);
size_t		ft_envlen(t_env *envh);
char		**struct_to_darr(t_env *envh, t_tool *tool);
void		here_docs(t_redir *redirs, t_tool *tool);
void		handle_herdocs(t_tree *tree, t_tool *tool);
int			ft_atoi(const char *str);
long long	ft_atol_ex(char *str, int sign, int i);
size_t		ft_dstrlen(const char **s);
char		**ft_split(char const *s, char c, t_tool *tool);
char		*ft_strjoin(char *s1, char *s2, t_tool *tool);
void		ft_putchar_fd(char c, int fd);
char		*ft_substr(char const *s, unsigned int start,
				size_t len, t_tool *tool);
char		*ft_strdup(const char *s1, t_tool *tool);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_isalpha(int a);
int			ft_isdigit(int n);
long long	ft_atoll(const char *str);
int			ft_isalnum(int c);
char		*ft_itoa(int n, t_tool *tool);
void		ft_putnbr_fd(int n, int fd);
char		*ft_substr_env(char const *s, unsigned int start, size_t len);
char		*ft_strdup_env(const char *s1, int flag);
char		*ft_itoa_env(int n);
char		*ft_strjoin_env(char *s1, char *s2, int flag);
void		print_err(char *msg1, char *arg, char *msg2);
char		*expand_herdoc_content(char *line, t_tool *tool, int status);
int			handle_special(t_expand *exp, t_tool *tool, char *str, int status);
char		*extract_var_name(t_expand *expand, t_tool *tool, char *str);
void		expand_env_variable(t_expand *exp, t_tool *tool, t_env *env_node);
int			handle_question(t_expand *expand, t_tool *tool, int status);
void		init_expand(t_expand *expand, t_tree *tree);
void		process_wildcard_expansion(t_expand *expand, t_tool *tool);
int			is_break_token(int type);
void		update_links(t_token **input, t_token *prev, t_token *target);
void		add_to_index(t_redir *after, int index);
void		ft_dup(int *io, int flag);
void		read_from_heredoc(t_redir *red, t_tool *tool, int status);
void		handle_child(t_redir *red, t_tool *tool, int fd[2]);
size_t		there_is_herdoc(t_redir *red);
int			execute_one(t_tree *cmd, t_env *envh, t_tool *tool);
int			cd_complex(t_env **envh, t_tree *cmd,
				char **pwd_backup, t_tool *tool);
int			count_heredocs(t_tree *tree);
int			manipulate_export(t_env **envh, t_tree *cmd, char *key, char *val);
int			is_only_space(char *str);
void		init_struct_tool_exec(t_tool *tool, t_env **envh, char **env);
void		fun_help(void);
void		init_struct_tool_exec(t_tool *tool, t_env **envh, char **env);
void		colse_all(void);
void		main_helper(t_tool *tool, char *line);
void		main_sigs(t_tool *tool);
int			is_only_whitespace(char *str);
void		up_key(char *key, char *val, t_env **envh);
void		fill_fds(int *fds, t_redir *red, t_tool *tool);
char		*generate_file(t_redir *red, t_tool *tool);
t_redir		*find_lasthd(t_redir *redirs);

#endif