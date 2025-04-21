/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:35:16 by sgmih             #+#    #+#             */
/*   Updated: 2025/04/21 14:17:57 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"

int	contains_wildcard(const char *word)
{
	int	i;

	i = 0;
	while (word[i])
	{
		if (word[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

char	*extract_quoted_string(const char *line, int *i, char quote_char)
{
	int		start;
	int		end;
	int		len;
	char	*result;

	start = *i + 1;
	end = start;
	while (line[end] && line[end] != quote_char)
		end++;
	if (!line[end])
	{
		printf("Error: Unclosed quote\n");
		return (NULL);
	}
	len = end - start;
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	while (--len >= 0)
		result[len] = line[start + len];
	result[end - start] = '\0';
	*i = end;
	return (result);
}

char	*extract_word(const char *line, int *i)
{
	int		start;
	int		end;
	int		len;
	char	*result;

	start = *i;
	end = start;
	while (line[end] && line[end] != ' ' && line[end] != '\t'
		&& line[end] != '|' && line[end] != '&'
		&& line[end] != '<' && line[end] != '>'
		&& line[end] != '(' && line[end] != ')'
		&& line[end] != '\'' && line[end] != '\"')
		end++;
	len = end - start;
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	while (--len >= 0)
		result[len] = line[start + len];
	result[end - start] = '\0';
	*i = end - 1;
	return (result);
}

t_token *parse_input(const char *line)
{
	t_token *token_list = NULL;
	int i;
	char *word;
	int has_wildcard;

	i = 0;
	while (line[i])
	{
		if (line[i] == ' ' || line[i] == '\t')
		{
			i++;
			continue;
		}
		
		// Handle quotes (single and double)
		if (line[i] == '\'' || line[i] == '\"')
		{
			char quote = line[i];
			char *quoted_str = extract_quoted_string(line, &i, quote);
			if (!quoted_str)
			{
				free_tokens(token_list);
				return NULL;
			}
			
			t_token *new_token = create_token(TOKEN_WORD, quoted_str);
			free(quoted_str);
			add_token(&token_list, new_token);
		}

		else if (line[i] == '|')
		{
			if (line[i + 1] == '|')
			{
				add_token(&token_list, create_token(TOKEN_OR, "||"));
				i++;
			}
			else
				add_token(&token_list, create_token(TOKEN_PIPE, "|"));
		}
		
		else if (line[i] == '&' && line[i + 1] == '&')
		{
			add_token(&token_list, create_token(TOKEN_AND, "&&"));
			i++;
		}

		else if (line[i] == '<')
		{
			if (line[i + 1] == '<')
			{
				add_token(&token_list, create_token(TOKEN_REDIR_HEREDOC, "<<"));
				i++;
			}
			else
				add_token(&token_list, create_token(TOKEN_REDIR_IN, "<"));
		}

		else if (line[i] == '>')
		{
			if (line[i + 1] == '>')
			{
				add_token(&token_list, create_token(TOKEN_REDIR_APPEND, ">>"));
				i++;
			}
			else
				add_token(&token_list, create_token(TOKEN_REDIR_OUT, ">"));
		}

		else if (line[i] == '(') {
			add_token(&token_list, create_token(TOKEN_PAREN_OPEN, "("));
		}
		else if (line[i] == ')') {
			add_token(&token_list, create_token(TOKEN_PAREN_CLOSE, ")"));
		}
		
		else
		{
			word = extract_word(line, &i);
			if (!word)
			{
				free_tokens(token_list);
				return (NULL);
			}
	
			has_wildcard = contains_wildcard(word);
			add_token(&token_list, create_token(TOKEN_WORD, word));
			free(word);
		}

		i++;
	}
	return (token_list);
}
