/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:13:04 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/13 15:35:01 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"

static int create_delim_token(char *cmd, int i, t_token **token, t_tool *tool)
{
    int type;
    t_token *new_token;

    type = is_delimter(cmd[i], cmd[i + 1]);

    if (type == TOKEN_SINGL_AND)
    {
        tool->anderr = 1; // Single & is an error
    }

    if (type == 2 || type == 3
        || type == 7 || type == 8)
    {
        new_token = lst_new(ft_my_strdup(&cmd[i], 2, tool), tool);
        new_token->type = type;
        lst_add_back(token, new_token);
        i += 2;
    }
    else
    {
        new_token = lst_new(ft_my_strdup(&cmd[i], 1, tool), tool);
        new_token->type = type;
        lst_add_back(token, new_token);
        i += 1; // Skip one character
    }

    return (i);
}

static int  create_space_token(t_token **token, char *cmd, int i, t_tool *tool)
{
	t_token	*new_token;
	char	*space_str;
	int		n;

	new_token = NULL;
	space_str = NULL;
	n = i;
	while (cmd[i] && is_space(cmd[i]))
		i++;
	space_str = ft_my_strdup(&cmd[n], i - n, tool);

	new_token = lst_new(space_str, tool);
    new_token->type = 4;
	new_token->priority = 0;
    lst_add_back(token, new_token);

	return (i);
}

static int	create_cmd_token(t_token **token, char *cmd, int i, t_tool *tool)
{
	t_token	*new_token;
	char	*word_str;
    int     end;
	t_tool temp_tool; 
	
	temp_tool = *tool;
	new_token = NULL;
	word_str = NULL;
    end = i;
	
    while (cmd[end] && (temp_tool.quoted || (!is_space(cmd[end]) && !is_delimter(cmd[end], cmd[end + 1]))))
        hundel_quotes_paren(&temp_tool, cmd[end++]);

    if (end != i)
    {
    	word_str = ft_my_strdup(&cmd[i], end - i, tool);
		new_token = lst_new(word_str, tool);
    	new_token->type = 0;
		new_token->priority = 0;
        lst_add_back(token, new_token);
		
        return (end);
    }
    return (i + 1);
}

t_token	*tokens_lst(char *cmd, t_tool *tool)
{
	t_token	*token;
	int	i;

	i = 0;
	token = NULL;
	while (cmd && cmd[i])
	{
		if (is_space(cmd[i]))
			i = create_space_token(&token, cmd, i, tool);
		else if (cmd[i] && (is_delimter(cmd[i], cmd[i + 1]) > 0) && tool->quoted == 0)
			i = create_delim_token(cmd, i, &token, tool);
		else
			i = create_cmd_token(&token, cmd, i, tool);
		update_token(&token, tool);
		if (token && token->type == TOKEN_WORD || is_delimter(cmd[i], cmd[i + 1]) == 0)
    		init_redir_file_tokens(token);
		init_quote_token(token);

		if (pars_err(&token, tool))
			return (NULL);
	}
	return (token);
}
