/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:56:56 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/17 08:21:33 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int one_wildcard(char *str)
{
	while (*str)
	{
		if (*str != '*')
			return (0);
		str++;
	}
	return (1);
}

void sort_list_matches(t_token **list_matches)
{
	t_token *current;
	t_token *next;
	char *temp;
	int swapped;

	if (!list_matches || !*list_matches)
		return;

	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		current = *list_matches;

		while (current && current->next)
		{
			next = current->next;
			if (ft_strcmp(current->value, next->value) > 0)
			{
				temp = current->value;
				current->value = next->value;
				next->value = temp;
				swapped = 1;
			}
			current = current->next;
		}
	}
}

void list_one_wildcard(t_token **list_matches, t_tool *tool)
{
	DIR				*dir;
	struct dirent	*entry_name;

	dir = opendir(".");
	if (dir == NULL)
	{
		perror("Error");
		return;
	}
	entry_name = readdir(dir);
	while (entry_name != NULL)
	{
		if (entry_name->d_name[0] != '.')
			lst_add_back(list_matches,
				new_lst(ft_strdup(entry_name->d_name, tool), tool));
		entry_name = readdir(dir);
	}
	if (closedir(dir) != 0)
		perror("Error");
}

int get_list(t_token **list_matches, char *wildcard, t_tool *tool)
{
	DIR				*dir;
	struct dirent	*entry_name;
	int				found_match;

	found_match = 0;
	dir = opendir(".");
	if (dir == NULL)
	{
		perror("Error");
		return (0);
	}
	entry_name = readdir(dir);
	while (entry_name != NULL)
	{
		if (entry_name->d_name[0] != '.' && get_list_match(entry_name->d_name, wildcard, tool))
		{
			lst_add_back(list_matches, new_lst(ft_strdup(entry_name->d_name, tool), tool));
			found_match = 1;
		}
		entry_name = readdir(dir);
	}
	if (closedir(dir) != 0)
		perror("Error");
	return (found_match);
}

static char	**convert_list_to_array(t_token *lst, int match_count, t_tool *tool)
{
	char	**list_wld;
	t_token	*current;
	int		i;

	list_wld = (char **)malloc(sizeof(char *) * (match_count + 1));
	if (!list_wld)
		return (NULL);
	add_to_grbg(&tool->grbg, list_wld);
	current = lst;
	i = 0;
	while (current)
	{
		list_wld[i] = ft_strdup(current->value, tool);
		i++;
		current = current->next;
	}
	list_wld[i] = NULL;
	return (list_wld);
}

char	**expand_wildcard(char *buff_exp, t_tool *tool)
{
	t_token	*list_matches;
	t_token	*current;
	int		match_count;

	list_matches = NULL;
	match_count = 0;
	if (one_wildcard(buff_exp))
	{
		list_one_wildcard(&list_matches, tool);
		sort_list_matches(&list_matches);
	}
	else
	{
		if (!get_list(&list_matches, buff_exp, tool))
			lst_add_back(&list_matches,
				new_lst(ft_strdup(buff_exp, tool), tool));
	}
	current = list_matches;
	while (current)
	{
		match_count++;
		current = current->next;
	}
	return (convert_list_to_array(list_matches, match_count, tool));
}
