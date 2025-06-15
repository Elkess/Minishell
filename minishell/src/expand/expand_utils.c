/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:54:25 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/15 10:26:08 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*strjoin_char(char *str, char c, t_tool *tool)
{
	size_t	len;
	size_t	i;
	char	*new_str;
	char	*old_str;

	old_str = str;
	if (!str)
	{
		new_str = malloc(sizeof(char) * 2);
		if (!new_str)
			return (NULL);
		add_to_grbg(&tool->grbg, new_str);
		new_str[0] = c;
		new_str[1] = '\0';
		return (new_str);
	}
	len = ft_strlen(str);
	new_str = malloc(sizeof(char) * (len + 2));
	if (!new_str)
		return (NULL);
	add_to_grbg(&tool->grbg, new_str);
	i = 0;
	while (i < len)
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = c;
	new_str[i + 1] = '\0';
	// free(old_str);
	return (new_str);
}

int	valid_char(char c)
{
	if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') || c == '_')
		return (1);
	return (0); 
}

int has_space(const char *str)
{
    if (!str)
        return (0);
    if (*str == ' ')
        return (2);
    while (*str)
    {
        if (*str == ' ')
            return (1);
        str++;
    }
    return (0);
}

char **create_cmd_array_2(t_token *token, t_tool *tool)
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
	add_to_grbg(&tool->grbg, result);
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
