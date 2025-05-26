/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:51:01 by melkess           #+#    #+#             */
/*   Updated: 2025/05/26 12:05:48 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void print_err(char *msg1, char *arg, char *msg2)
{
	// store backup first
	int	fd = dup(1);
	dup2(2, 1);
	if (!msg1)
		printf("minishell: %s: %s\n", arg, msg2);
	else
		printf("minishell: %s%s: %s\n", msg1, arg, msg2);
	dup2(fd, 1);
}

char	**struct_to_darr(t_env *envh)
{
	size_t	len;
	t_env	*tmp;
	char	**env;

	len = ft_envlen(envh);
	env = malloc((len +1) * sizeof(char *));
	if (!env)
		(free_envh(envh), exit (1));
	len = 0;
	while (envh)
	{
		if (envh->value)
		{
			env[len] = ft_strjoin(envh->key, "=", 0);
			// free tmp of before env[len]
			env[len] = ft_strjoin(env[len], envh->value, 1);
			len++;
		}
		envh = envh->next;
	}
	env[len] = NULL;
	return (env);
}

void	is_dir(char **p, char *path)
{
	struct stat	s;

	if (!ft_strcmp(path, "."))
		(print_err(NULL, path, ": command not found"), exit(127)); // SHoud it be exit and free_ evnh ??? exit YES
	if (!stat(path, &s) && S_ISDIR(s.st_mode))
	{
		if ((p && *p && !ft_strcmp(path, "..")) || !ft_strchr(path, '/'))
			(print_err(NULL, path, ": command not found"),exit(127)); // SHoud it be exit and free_ evnh ??? exit YES
		else
			(print_err(NULL, path, ": is a directory"), exit(126)); // SHoud it be exit and free_ evnh ??? exit YES
	}
}

void	exec_helper(char **cmd, char **env, t_env *envh, char **path)
{
	size_t	i;
	char	*new_path;

	new_path = NULL;
	i = 0;
	if (!ft_strchr(cmd[0], '/'))
	{
		while (path && path[i])
		{
			new_path = ft_strjoin(path[i], "/", 0);
			// free before path[i] in tmp
			new_path = ft_strjoin(new_path, cmd[0], 1);
			if (!access(new_path, X_OK))
			{
				if (execve(new_path, cmd, env) == -1)
					(perror("Execve2 Failed:"), exit(1)); // SHoud it be exit and free_ evnh ??? exit
			}
			free(new_path);
			i++;
		}
	}
}

void	execute_one(t_tree *cmd, t_env *envh)
{
	pid_t		fd = fork();
	char		**env;
	char		**path;

	path = NULL;
	env = NULL;
	if (search_for_defaults(envh, "PATH"))
		path = ft_split(search_for_defaults(envh, "PATH")->value, ':');
	env = struct_to_darr(envh);
	if (fd == -1)
		(perror("Fork1 Failed"), exit(1)); // SHoud it be exit and free_ evnh ??? exit
	if (fd == 0)
	{
		is_dir(path, cmd->cmd[0]);
		if ((!access(cmd->cmd[0], X_OK)))
		{
			if (execve(cmd->cmd[0], cmd->cmd, env) == -1)
				(perror("Execve1 Failed:"), exit(1));// SHoud it be exit and free_ evnh ??? exit
		}
		exec_helper(cmd->cmd, env, envh, path);
		if (((errno == 20 || errno == 13 || errno == 2) && ft_strchr(cmd->cmd[0], '/')) || !path)
			(print_err(NULL, cmd->cmd[0], strerror(errno)), exit (126 * (errno != 2) + 127 * (errno == 2)));
		(print_err(NULL, cmd->cmd[0], ": command not found"), exit(127));// SHoud it be exit and free_ evnh ??? exit
	}
	(free_twod(path), free_twod(env));
}

void	ft_dup(int *io, int flag)
{
	if (flag)
	{
		io[0] = dup(0);
		io[1] = dup(1);
		if (io[0] == -1 || io[1] == -1)
			(close(io[0]), close(io[1]));
	}
	else
	{
		if (dup2(io[0], 0) == -1 || dup2(io[1], 1) == -1)
			(close(io[0]), close(io[1]));
		(close(io[0]), close(io[1]));
	}
}

t_redir	*find_lastredir(t_redir *redirs, t_redir_type type)
{
	t_redir	*last;

	last = NULL;
	if (type == REDIR_OUT || type == REDIR_APPEND)
	{
		while (redirs)
		{
			if (redirs->type == REDIR_OUT || redirs->type == REDIR_APPEND)
				last = redirs;
			redirs = redirs->next;
		}
	}
	if (type == REDIR_IN || type == REDIR_HEREDOC)
	{
		while (redirs)
		{
			if (redirs->type == REDIR_IN || redirs->type == REDIR_HEREDOC)
				last = redirs;
			redirs = redirs->next;
		}
	}
	return (last);
}

int	handle_lastredir(t_redir *redirs)
{
	t_redir	*lastin;
	t_redir	*lastout;
	lastin = find_lastredir(redirs, REDIR_IN);
	lastout = find_lastredir(redirs, REDIR_OUT);

	if (lastin && lastin->type != REDIR_HEREDOC && (!lastout ||
		(lastout && lastin->index < lastout->index)))
	{
		lastin->fd = open(lastin->file, O_RDONLY);
		if (lastin->fd == -1)
			return (print_err(NULL, lastin->file, strerror(errno)), 1);
	}
	if (lastout && lastout->type == REDIR_OUT)
		lastout->fd = open(lastout->file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	else if (lastout && lastout->type == REDIR_APPEND)
		lastout->fd = open(lastout->file, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (lastout && lastout->fd == -1)
		return (print_err(NULL, lastout->file, strerror(errno)), 1);
	if (lastin && lastin->type != REDIR_HEREDOC && lastout && lastin->index > lastout->index)
	{
		lastin->fd = open(lastin->file, O_RDONLY);
		if (lastin->fd == -1)
			return (print_err(NULL, lastin->file, strerror(errno)), 1);
	}
	if (lastin)
		dup2(lastin->fd, 0);
	if (lastout) 
		dup2(lastout->fd, 1);
	return (0);
}

int	ft_redir(t_tree *tree)
{
	t_redir	*red;

	if (!tree || !tree->redirs)
        return (0);
		
	red = tree->redirs;
	while (red)
	{
		// if (red != find_lastredir(tree->redirs, red->type) && red->type != REDIR_HEREDOC)
		// {
			if (red->type == REDIR_IN)
				red->fd = open(red->file, O_RDONLY);
			else if (red->type == REDIR_OUT)
				red->fd = open(red->file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
			else if (red->type == REDIR_APPEND)
				red->fd = open(red->file, O_CREAT | O_APPEND | O_WRONLY, 0644);
			if (red->fd == -1)
			{
				print_err(NULL, red->file, strerror(errno));
				return (1);
			}
		// }
		if (red != find_lastredir(tree->redirs, REDIR_OUT) && // opened fd
			red != find_lastredir(tree->redirs, REDIR_IN))
			close(red->fd);
		red = red->next;
	}
	return (handle_lastredir(tree->redirs));
}

char	*generate_file(t_redir *red)
{
	char	*str;
	size_t	i;

	str = NULL;
	i = 0;
	while (1)
	{
		free (str);
		str = ft_strjoin("/tmp/.here_doc", ft_itoa(i), 2); //leaks
		if (access(str, F_OK) != 0)
		{
			red->fd = open(str, O_CREAT | O_RDWR | O_APPEND);
			if (red->fd == -1)
				(perror("Open failed in << :"),free(str), exit (1));// SHoud it be exit and free_ evnh ??? exit
			break ;
		}
		i++;
	}
	return (str);
}

void	here_docs(t_redir *red, t_env *envh)
{
	char	*line;
	char	*file;

	line = NULL;
	while (red)
	{
		if (red->type == REDIR_HEREDOC)
		{
			file = generate_file(red);
			while (1)
			{
				line = readline("> ");
				if (!line)
					break ;
				if (!ft_strcmp(line, red->file))
				{
					free(line);
					break ;
				}
				// if (red->flag == 1)
				// {
				// 	line = heredoc_expand(line);
				// }
				line = ft_strjoin(line, "\n", 1);
				write(red->fd, line, ft_strlen(line));
				free(line);
			}
			close(red->fd);
			///
			red->fd = open(file, O_CREAT | O_RDWR | O_APPEND);
			// read it and expand each line
			if (red->fd == -1)
				(perror("Open failed in 272:"), free_envh(envh),free(file), exit (1)); // add free file
			unlink(file);
			free(file);
		}
		red = red->next;
	}
}

t_redir	*find_lasthd(t_redir *redirs)
{
	t_redir	*last;

	last = NULL;
	while (redirs)
	{
		if (redirs->type == REDIR_HEREDOC)
			last = redirs;
		redirs = redirs->next;
	}
	return (last);
}

int	is_builtin(t_tree *tree, char	*cmd, t_env *envh)
{
	static char	*pwd_backup;

	if (!ft_strcmp(cmd, "echo"))
		return(echo(tree));
	else if (!ft_strcmp(cmd, "cd"))
		return(cd(&envh, tree, &pwd_backup));
	else if (!ft_strcmp(cmd, "export"))
		return(ft_export(&envh, tree));
	else if (!ft_strcmp(cmd, "unset"))
		return(unset(&envh, tree->cmd));
	else if (!ft_strcmp(cmd, "env"))
		return(env(envh));
	else if (!ft_strcmp(cmd, "pwd"))
		return(pwd(&pwd_backup));
	else if (!ft_strcmp(cmd, "exit"))
		ft_exit(tree, 0, envh);
	return (-1);
}



int	execute_cmd(t_tree *tree, t_env *envh, int status)
{
	char	*cmd;
	if (tree && tree->cmd)
	{

		cmd = tree->cmd[0];
		if (!cmd)
			return (0);
		status = is_builtin(tree, cmd, envh);
		if (status == -1)
		{
			(execute_one(tree, envh), waitpid(0, &status, 0));
			status = WEXITSTATUS(status);
		}
	}
	return (status);
}





/**********************************************************************************************************************************************************************/

typedef struct s_expand {
    int		flg;
	int		i;
	int		j;
	int		is_wildcard;
    int		before_qoute;
    int		is_char;
    char	*buff_exp;
    t_token	*token;
} t_expand;

size_t	ft_strlen2(const char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = 0;
	while (s[len])
		len++;
	return (len);
}

char	*ft_strdup2(const char *s1)
{
	size_t	len;
	size_t	i;
	char	*ptr;

	if (!s1)
		return (NULL);
	len = ft_strlen2(s1);
	ptr = malloc(sizeof(char) * (len + 1));
	if (ptr == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		ptr[i] = s1[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

char	*strjoin_char(char *str, char c)
{
	size_t	len;
	size_t	i;
	char	*new_str;
	char	*old_str = str;

	if (!str)
	{
		new_str = malloc(sizeof(char) * 2);
		if (!new_str)
			return (NULL);
		new_str[0] = c;
		new_str[1] = '\0';
		return (new_str);
	}
	len = ft_strlen2(str);
	new_str = malloc(sizeof(char) * (len + 2));
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = c;
	new_str[i + 1] = '\0';

	free(old_str);
	
	return (new_str);
}

t_token	*new_lst(void *content)
{
	t_token	*new_node;

	if (!content)
		return (NULL);
	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->value = content;
	new_node->next = NULL;
	return (new_node);
}

void free_token_list(t_token *token)
{
	t_token *current = token;
	t_token *next;
	
	while (current)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}

char **create_cmd_array_2(t_token *token)
{
    t_token *current;
    char **result;
    int count;
    int i;

	if (!token)
        return (NULL);
    count = 0;
    current = token;
    while (current)
    {
        count++;
        current = current->next;
    }

    result = malloc(sizeof(char *) * (count + 1));
    if (!result)
        return (NULL);
    current = token;
    i = 0;
    while (current && i < count)
    {
        result[i] = current->value;
        current = current->next;
        i++;
    }
    result[i] = NULL;

    return (result);
}

static void expand_quote(t_expand *expand, char *str, int status)
{
    if (expand->flg == 0)
    {
        if (expand->j == 0 || is_space(str[expand->j - 1]))
            expand->before_qoute = 1;
        expand->flg = str[expand->j];
    }
    else if (expand->flg == str[expand->j])
    {
        if (expand->before_qoute && str[expand->j - 1] == expand->flg
            && (str[expand->j + 1] == '\0' || is_space(str[expand->j + 1])))
			lst_add_back(&expand->token, new_lst(ft_strdup2("")));
        else if (expand->is_char && expand->buff_exp)
        {
            lst_add_back(&expand->token, new_lst(ft_strdup2(expand->buff_exp)));
			free(expand->buff_exp);
            expand->buff_exp = NULL;
        }
        expand->flg = 0;
        expand->before_qoute = 0;
		expand->is_char = 0; 
    }
    else
    {
        expand->buff_exp = strjoin_char(expand->buff_exp, str[expand->j]);
        expand->is_char = 1;
    }
}

static void expand_to_buff(t_expand *expand, char *str)
{
	expand->buff_exp = strjoin_char(expand->buff_exp, str[expand->j]);
	// if (str[expand->j] != '*')
	// 	expand->is_char = 1;
	// else
	// 	expand->is_wildcard = 1;
	expand->is_char = 1;
	// printf("\033[34m expand_to_buff, quote = '%c', buff_exp = '%s', token = %p \033[0m\n",
    //     expand->flg ? expand->flg : '0', expand->buff_exp ? expand->buff_exp : "(null)", expand->token);
}

static int	valid_char(char c)
{
	if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9'))
		return (1);
	return (0); 
}

/************************************************** function itoa ***********************************/
static int	count_len(int n)
{
	int	len;

	if (n == 0)
		return (1);
	len = 0;
	if (n < 0)
		len++;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static void	special_case(int *n, char *result, int *i)
{
	result[0] = '-';
	result[1] = '2';
	*n = 147483648;
	(*i)++;
}

char	*ft_itoa2(int n)
{
	int		len;
	int		i;
	char	*result;

	len = count_len(n);
	result = (char *)malloc((len + 1) * sizeof(char));
	if (!result)
		return (NULL);
	result[len] = '\0';
	i = 0;
	if (n < 0)
	{
		result[0] = '-';
		n = -n;
		i++;
	}
	if (n == -2147483648)
		special_case(&n, result, &i);
	while (len > i)
	{
		len--;
		result[len] = '0' + (n % 10);
		n /= 10;
	}
	return (result);
}

/************************************************** function itoa ***********************************/

char *strjoin_2(char *s1, char *s2)
{
    size_t len1 = ft_strlen2(s1);
    size_t len2 = ft_strlen2(s2);
    char *result = malloc(len1 + len2 + 1);
    size_t i = 0;

    if (!result)
        return (NULL);
    if (s1)
    {
        while (i < len1)
        {
            result[i] = s1[i];
            i++;
        }
    }
    if (s2)
    {
        while (i < len1 + len2)
        {
            result[i] = s2[i - len1];
            i++;
        }
    }
    result[i] = '\0';
    return (result);
}

static void expand_dollar(t_expand *expand, t_env *env, char *str, int status)
{
    char *expanded_value = NULL;
    char *var_name = NULL;
    t_env *env_node = NULL;
	char *temp;

    expand->j++; // Skip the '$'

    if (str[expand->j] == '?')
    {
        expanded_value = ft_itoa2(status);
        expand->j++;
    }
    else if (str[expand->j] == '$')
    {
        expanded_value = ft_strdup2("$");
        expand->j++;
    }
    else if (str[expand->j] == '0')
    {
        expanded_value = ft_strdup2("minishell");
        expand->j++;
    }
    else
    {
        // Collect variable name into a separate buffer
        while (str[expand->j] && valid_char(str[expand->j]))
        {
            var_name = strjoin_char(var_name, str[expand->j]);
            expand->j++;
        }
        if (var_name)
        {
            env_node = search_for_defaults(env, var_name);
			if (env_node && env_node->value)
                expanded_value = ft_strdup2(env_node->value);
            else
                expanded_value = ft_strdup2("");
            free(var_name);
        }
    }

    if (expanded_value)
    {
        if (expand->flg == '"')
        {
            temp = expand->buff_exp;
            expand->buff_exp = strjoin_2(temp, expanded_value); 
            free(temp);
            expand->is_char = 1;
        }
        else
            lst_add_back(&expand->token, new_lst(ft_strdup2(expanded_value)));
        free(expanded_value);
    }
}


char	**handel_expand(t_tree *tree, t_env *env, int exit_status)
{
	t_expand expand;

	if (!tree || !tree->cmd)
		return (NULL);
		
	expand.i = 0;
	expand.j = 0;
	expand.flg = 0;
	expand.before_qoute = 0;
	expand.is_char = 0;
	expand.token = NULL;
	expand.buff_exp = NULL;
		
	while (tree->cmd[expand.i])
	{
		expand.j = 0;
		expand.is_char = 0;

		if (!tree->cmd[expand.i])
		{
			expand.i++;
			continue;
		}
		
		while (tree->cmd[expand.i][expand.j])
		{
			if (tree->cmd[expand.i][expand.j] == '"' || tree->cmd[expand.i ][expand.j] == '\'')
			{
				expand_quote(&expand, tree->cmd[expand.i], exit_status);
				//printf("\033[33m Executing qourtes [%d][%d] : %c \033[0m\n", expand.i , expand.j, tree->cmd[expand.i][expand.j]);
				expand.j++;
			}
			else if (tree->cmd[expand.i][expand.j] == '$' && expand.flg != '\'')
			{
				expand_dollar(&expand, env, tree->cmd[expand.i], exit_status);
				//printf("\033[33m expand_dollar [%d][%d] : %c \033[0m\n", expand.i , expand.j, tree->cmd[expand.i][expand.j]);
			}
			else
			{
				expand_to_buff(&expand, tree->cmd[expand.i]);
				//printf("\033[33m cmd_rest [%d][%d] : %c \033[0m\n", expand.i , expand.j, tree->cmd[expand.i][expand.j]);
				expand.j++; 
			}
		}
		// TODO: Save expand.buff_exp into a result array here if needed

		if (expand.buff_exp && expand.is_char)
        {
            lst_add_back(&expand.token, new_lst(ft_strdup2(expand.buff_exp)));
			free(expand.buff_exp);
            expand.buff_exp = NULL;
            expand.is_char = 0;
        }
		expand.i++;
	}

	char **result = create_cmd_array_2(expand.token);
	// printf("DEBUG: Created result array: %p\n", result);
    // if (result)
    // {
    //     for (int i = 0; result[i]; i++)
    //         printf("DEBUG: result[%d] = '%s'\n", i, result[i]);
    // }
	
	return (result);
}


/******************************************************************************************/

int	executor(t_tree *tree, t_env *envh)
{
	// TODO: handle tty && signals 
	int		fds[2];
	int		redir_status = 0;
	int		status;
	t_redir	lasthd;
	char **expanded_cmd;

	status = 0;

    if (tree && tree->cmd)
    {
        for (int i = 0; tree->cmd[i]; i++)
            printf("\033[32m executor: Initial tree->cmd[%d] = '%s' \033[0m\n", i, tree->cmd[i]);
    }
    else
        printf("\033[32m executor: tree->cmd is NULL or tree is NULL \033[0m\n");
		
		 
	expanded_cmd = handel_expand(tree, envh, status);
	
    if (expanded_cmd)
    {
        for (int i = 0; expanded_cmd[i]; i++)
            printf("\033[32m executor: expanded_cmd[%d] = '%s' \033[0m\n", i, expanded_cmd[i]);
    }

	// if (expanded_cmd)  // tree->cmd double-free 
    //     tree->cmd = expanded_cmd;

	// //exit(1);
	// if (!tree || tree->type != NODE_COMMAND && tree->type != NODE_PARENTHS)
	// 	return (1);
	// ft_dup(fds, 1);
	// if (tree->redirs)
	// {
	// 	// lasthd = find_lasthd(tree->redirs);
	// 	// TODO: 2 handle herdoc properly this is wrong
	// 	// if (lasthd)
	// 	// 	here_docs(tree->redirs);
	// 	redir_status = ft_redir(tree);
	// }
	// if (!redir_status && tree->type == NODE_COMMAND)
	// 	status = execute_cmd(tree, envh, status);
	// else if (!redir_status && tree->type == NODE_PARENTHS)
	// 	status = execute_tree(tree->left, envh);
	// ft_dup(fds, 0);
	// if (!tree->cmd)
	// 	return (redir_status);
	return (status);
}
