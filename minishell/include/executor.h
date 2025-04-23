#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "../include/minishell.h"

int	executor(t_tree *tree,t_env	*envh);

// builts in
void	echo(t_tree *cmd1);
void	pwd(int fd);
void	env(t_env *envh);
void	cd(t_env **envh, t_tree *cmd);
void	export(t_env **envh, t_tree *cmd);
void	unset(t_env **envh, char **args);
void    ft_exit(t_tree *cmd);

// libft
int	ft_atoi(const char *str);
long long	ft_atol(const char *str,int sign);
char	*ft_substr(char const *s, unsigned int start, size_t len);
size_t	ft_strlen(const char *s);
size_t	ft_dstrlen(const char **s);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
void	ft_putstr_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);
char	*ft_strdup(const char *s1);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_isalpha(int a);
int		ft_isdigit(int n);
int		ft_isalnum(int c);
char	*ft_strchr(const char *s, int c);

// void	print_rest(char **s, int fd, int n);
t_env	*fill_env(char **envp);
char	*extract_data(char *line, int flag);
t_env	*append_node(t_env *head, char *key, char *val);
t_env	*search_for_defaults(t_env *envh, char *key);
int     is_valid_key(char *key);
t_env	*edit_env(char *key, char *val, t_env *envh, int should_concat);
void	print_rest(char **s, int fd, int n);

#endif