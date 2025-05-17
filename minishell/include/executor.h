#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "../include/minishell.h"

# include <unistd.h>
# include <limits.h>
#include <string.h>
#include <dirent.h>
# include <fcntl.h>


typedef struct	s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;

int	execute_tree(t_tree *tree, t_env *envh);
int	executor(t_tree *tree,t_env	*envh);

// builts in
int		echo(t_tree *cmd1);
int		pwd(char **cd_path, int print);
int		env(t_env *envh);
int		cd(t_env **envh, t_tree *cmd, char	**fake_pwd);
int		ft_export(t_env **envh, t_tree *tree);
int		unset(t_env **envh, char **args);
void    ft_exit(t_tree *cmd, int status);

// void	print_rest(char **s, int fd, int n);
t_env	*fill_env(char **envp);
char	*extract_data(char *line, int flag);
t_env	*append_node(t_env *head, char *key, char *val);
t_env	*search_for_defaults(t_env *envh, char *key);
int     is_valid_key(char *key);
t_env	*edit_env(char *key, char *val, t_env *envh, int should_concat);
void	print_rest(char **s, int n);
size_t	ft_envlen(t_env *envh);
char	**struct_to_darr(t_env *envh);

// function libft
int			 ft_atoi(const char *str);
long long	ft_atol(const char *str,int sign);



//
void here_docs(t_redir *redirs);

#endif