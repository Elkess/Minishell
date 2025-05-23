/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 11:49:41 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/23 08:28:10 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "executor.h"
# include "parser.h"

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
// # include <dirent.h>
# include <sys/stat.h>
# include <errno.h>

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
	int				fd;
	int				flag;
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

typedef struct s_tree
{
	t_node_type		type;
	char			**cmd;
	t_redir			*redirs;
	t_redir			*redirs_before;
	t_redir			*redirs_after;
	int				is_ambiguous;
	struct s_tree	*left;
	struct s_tree	*right;
}	t_tree;

/* Libft functions used both us */
size_t	ft_strlen(const char *s);
void	ft_putstr_fd(char *s, int fd);
char	*ft_strchr(const char *s, int c);
int		ft_strcmp(const char *s1, const char *s2);

#endif