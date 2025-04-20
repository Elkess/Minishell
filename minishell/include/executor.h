#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "../include/minishell.h"

# include <stdio.h>
# include <stdlib.h>
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

int	executor(t_tree *tree,t_env	*envh);

// builts in
void	echo(t_tree *cmd1);
void	pwd(int fd);
void	env(t_env *envh);
void	cd(t_env **envh, t_tree *cmd);
void	export(t_env **envh, t_tree *cmd);
void	unset(t_env **envh, char **args);
void    ft_exit(t_tree *cmd);

// void	print_rest(char **s, int fd, int n);
t_env	*fill_env(char **envp);
char	*extract_data(char *line, int flag);
t_env	*append_node(t_env *head, char *key, char *val);
t_env	*search_for_defaults(t_env *envh, char *key);
int     is_valid_key(char *key);
t_env	*edit_env(char *key, char *val, t_env *envh, int should_concat);
void	print_rest(char **s, int fd, int n);

#endif