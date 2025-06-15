/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_help.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 10:42:17 by sgmih             #+#    #+#             */
/*   Updated: 2025/06/15 11:00:39 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	resullt_tab(char **dp, int str_len, int wildcard_len)
{
	int	result;
	int	i;

	if (dp[str_len][wildcard_len] == '1')
		result = 1;
	else
		result = 0;
	return (result);
}

static char	**creat_table(int rows, int cols, t_tool *tool)
{
	char	**tab;
	int		i;
	int		j;

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

static void	handle_first_column(char **tab, int i, int j)
{
	if (i == 0)
		tab[i][j] = '1';
	else
		tab[i][j] = '0';
}

static void	init_table_row(char **tab, int i, char *wildcard, char *str)
{
	int	j;
	int	wildcard_len;

	wildcard_len = ft_strlen(wildcard);
	j = 0;
	while (j <= wildcard_len)
	{
		if (j == 0)
		{
			handle_first_column(tab, i, j);
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
}

int	get_list_match(char *str, char *wildcard, t_tool *tool)
{
	char	**tab;
	int		str_len;
	int		wildcard_len;
	int		i;

	str_len = ft_strlen(str);
	wildcard_len = ft_strlen(wildcard);
	tab = creat_table(str_len, wildcard_len, tool);
	if (!tab)
		return (0);
	i = 0;
	while (i <= str_len)
	{
		init_table_row(tab, i, wildcard, str);
		i++;
	}
	return (resullt_tab(tab, str_len, wildcard_len));
}
