/* *********************************************************************************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lst_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:26:01 by sgmih             #+#    #+#             */
/*   Updated: 2025/05/10 16:05:18 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"

static int has_concat_quotes(char *value)
{
    int i = 0;

    while (value && value[i])
    {
        if ((value[i] == '\'' || value[i] == '"') && 
            (i == 0 || (i > 0 && value[i - 1] != '\\')))
        {
            return (1);
        }
        i++;
    }
    return (0);
}

static void init_quote_token(t_token *token)
{
    t_token *current;
	 char *val;

	current = token;
    while (current)
    {
        if (current->type == TOKEN_WORD)
        {
            val = current->value;
            size_t len = strlen(val);

            if (len >= 2 && val[0] == val[len - 1] && (val[0] == '\'' || val[0] == '"'))
            {
                if (val[0] == '\'')
                    current->type = 16;
                else
                    current->type = 17;
            }
            else if (has_concat_quotes(val))
            {
                current->type = 18;
            }
        }
        current = current->next;
    }
}

t_token	*check_token(t_token **token, t_tool *tool)
{
    t_token *tmp = *token;
    printf("Token list:\n");
    while (tmp)
    {
        printf("  type: %d, val: '%s'\n", tmp->type, tmp->value);
        tmp = tmp->next;
    }
	init_quote_token(*token);
    //print_tool(*tool);
	if (pars_err(token, tool))
		return (NULL);
	//update_lst_redir for fill s_redir 
	// pars_trime(*token, tool);
	return (*token);
}
