#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <stdlib.h>

typedef struct s_tree t_tree;
typedef struct s_redir t_redir;

typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
    struct s_env    *prev;
} t_env;

// Executor functions 
int     execute_tree(t_tree *tree, t_env *envh);
int     executor(t_tree *tree, t_env *envh);

// Built-in commands 
int     echo(t_tree *cmd1);
int     pwd(char **cd_path, int print);
int     env(t_env *envh);
int     cd(t_env **envh, t_tree *cmd, char **fake_pwd);
int     ft_export(t_env **envh, t_tree *tree);
int     unset(t_env **envh, char **args);
void    ft_exit(t_tree *cmd, int status);

//
t_env   *fill_env(char **envp);
char    *extract_data(char *line, int flag);
t_env   *append_node(t_env *head, char *key, char *val);
t_env   *search_for_defaults(t_env *envh, char *key);
int     is_valid_key(char *key);
t_env   *edit_env(char *key, char *val, t_env *envh, int should_concat);
void    print_rest(char **s, int n);
size_t  ft_envlen(t_env *envh);
char    **struct_to_darr(t_env *envh);
void    here_docs(t_redir *redirs);

// Libft functions for executor 
int     ft_atoi(const char *str);
long long ft_atol(const char *str, int sign);
size_t  ft_dstrlen(const char **s);
char    **ft_split(char const *s, char c);
char    *ft_strjoin(char const *s1, char const *s2);
void    ft_putchar_fd(char c, int fd);
char    *ft_substr(char const *s, unsigned int start, size_t len);
char    *ft_strdup(const char *s1);
int     ft_strncmp(const char *s1, const char *s2, size_t n);
int     ft_isalpha(int a);
int     ft_isdigit(int n);
int     ft_isalnum(int c);
char    *ft_itoa(int n);

#endif