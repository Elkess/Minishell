/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 11:46:59 by melkess           #+#    #+#             */
/*   Updated: 2025/06/15 11:07:48 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*extract_line(int fd, char	*holder)
{
	int		reads;
	char	*buff;
	char	*temp;

	buff = malloc(BUFFER_SIZE + 1 * sizeof(char));
	if (!buff)
		return (free(holder), holder = NULL);
	reads = 1;
	while (reads && !ft_strchr(holder, '\n'))
	{
		reads = read(fd, buff, BUFFER_SIZE);
		if (reads < 0)
			return (free(buff), free(holder), holder = NULL);
		buff[reads] = '\0';
		temp = holder;
		holder = ft_strjoin_env(holder, buff, 0);
		free(temp);
	}
	free(buff);
	return (holder);
}

static char	*filter_line(char *holder)
{
	size_t	i;

	if (*holder == '\0')
		return (NULL);
	i = 0;
	while (holder[i] != '\n' && holder[i] != '\0')
		i++;
	return (ft_substr_env(holder, 0, i +1));
}

static char	*ft_remainder(char *holder)
{
	char		*updated_holder;
	size_t		start;
	size_t		i;

	start = 0;
	while (holder[start] && holder[start] != '\n')
		start++;
	if (!holder[start])
		return (free(holder), holder = NULL);
	start++;
	i = 0;
	while (holder[start + i])
		i++;
	updated_holder = (char *)malloc((i + 1) * sizeof(char));
	if (!updated_holder)
		return (free(holder), holder = NULL);
	i = 0;
	while (holder[start])
		updated_holder[i++] = holder[start++];
	updated_holder[i] = '\0';
	free(holder);
	return (updated_holder);
}

char	*get_next_line(int fd)
{
	static char	*holder[OPEN_MAX];
	char		*line;

	if (fd < 0 || fd > OPEN_MAX)
		return (NULL);
	line = NULL;
	holder[fd] = extract_line(fd, holder[fd]);
	if (!holder[fd])
		return (NULL);
	line = filter_line(holder[fd]);
	if (!line)
		return (free(holder[fd]), holder[fd] = NULL);
	holder[fd] = ft_remainder(holder[fd]);
	return (line);
}
