/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:23:47 by melkess           #+#    #+#             */
/*   Updated: 2025/04/20 09:15:12 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor.h"

void	pwd(int fd)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		(perror("PWD Failed !"), exit (169));
	(ft_putstr_fd(pwd, fd));
	(ft_putstr_fd("\n", fd));
}