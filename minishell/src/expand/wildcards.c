/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkess <melkess@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:56:56 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/15 10:00:47 by melkess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int resullt_tab(char **dp, int str_len, int wildcard_len)
{
    int result;
    int i;
    
    if (dp[str_len][wildcard_len] == '1')
        result = 1;
    else
        result = 0;
    return (result);
}

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
            lst_add_back(list_matches, new_lst(ft_strdup(entry_name->d_name, tool), tool));
        entry_name = readdir(dir);
    }
    if (closedir(dir) != 0)
        perror("Error");
}

static char **creat_table(int rows, int cols, t_tool *tool)
{
    char **tab;
    int i;
    int j;

    tab = (char **)malloc(sizeof(char *) * (rows + 1));
    if (!tab)
        return (NULL);
    add_to_grbg(&tool->grbg, tab);
    i = 0;
    while (i <= rows)
    {
        tab[i] = (char *)malloc(sizeof(char) * (cols + 1));
        if (!tab[i])
            return (NULL);
        add_to_grbg(&tool->grbg, tab[i]);
        j = 0;
        while (j <= cols)
        {
            tab[i][j] = '0';
            j++;
        }
        i++;
    }

    tab[0][0] = '1';
    return (tab);
}

int get_list_match(char *str, char *wildcard, t_tool *tool)
{
    char **tab;
    int str_len;
    int wildcard_len;
    int i;
    int j;

    str_len = ft_strlen(str);
    wildcard_len = ft_strlen(wildcard);
    
    tab = creat_table(str_len, wildcard_len, tool);
    if (!tab)
        return (0);

    i = 0;
    while (i <= str_len)
    {
        j = 0;
        while (j <= wildcard_len)
        {
            if (j == 0)
            {
                if (i == 0)
                    tab[i][j] = '1';
                else
                    tab[i][j] = '0';
            }
            else if (wildcard[j - 1] == '*')
            {
                if (tab[i][j - 1] == '1' || (i > 0 && tab[i - 1][j] == '1'))
                    tab[i][j] = '1';
                else
                    tab[i][j] = '0';
            }
            else if (i > 0 && wildcard[j - 1] == str[i - 1])
                tab[i][j] = tab[i - 1][j - 1];
            else
                tab[i][j] = '0';
            j++;
        }
        i++;
    }
    return (resullt_tab(tab, str_len, wildcard_len));
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

char **expand_wildcard(char *buff_exp, t_tool *tool)
{
	t_token	*list_matches = NULL;
	char	**list_wld;
	t_token	*current;
	int		match_count = 0;
	int i;
    
    if (one_wildcard(buff_exp))
    {
        list_one_wildcard(&list_matches, tool);
        sort_list_matches(&list_matches);
    }
	else
    {
        if (!get_list(&list_matches, buff_exp, tool))
            lst_add_back(&list_matches, new_lst(ft_strdup(buff_exp, tool), tool));
    }
    current = list_matches;
    while (current)
    {
        match_count++;
        current = current->next;
    }
    list_wld = (char **)malloc(sizeof(char *) * (match_count + 1));
    if (!list_wld)
        return (NULL);
    add_to_grbg(&tool->grbg, list_wld);
	current = list_matches;
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