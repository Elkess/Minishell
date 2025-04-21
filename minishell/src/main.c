/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:21:18 by sgmih             #+#    #+#             */
/*   Updated: 2025/04/21 14:07:58 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parser.h"

char	*ft_get_prompt(void)
{
	char	*prompt;

	prompt = readline("minishell$> ");
	if (prompt && prompt[0])
		add_history(prompt);
	if (!prompt)
	{
		printf("exit\n");
		exit(0);
	}
	return (prompt);
}

void print_tokens(t_token *head) {
    t_token *current = head;
    int i = 0;
    const char *type_names[] = {
        "WORD", "PIPE", "AND", "OR", "REDIR_IN", "REDIR_OUT", 
        "REDIR_APPEND", "REDIR_HEREDOC", "PAREN_OPEN", "PAREN_CLOSE"
    };
    
    while (current) {
        printf("Token %d: Type=%s, Value='%s'\n", 
               i++, 
               type_names[current->type], 
               current->value);
        current = current->next;
    }
}

void handle_input(const char *line)
{
    t_token *tokens;
    
    tokens = parse_input(line);

    if (tokens) {
        print_tokens(tokens);
        free_tokens(tokens);
    } else {
        printf("Tokenization failed.\n");
    }
}

int	main(int ac, char **av, char **env)
{
	char	*line;
    
	(void)ac;
	(void)av;
	(void)env;

	while (1)
	{
		line = ft_get_prompt();
		printf("Line input: %s\n", line);

		handle_input(line);
		
	}
	return (0);
}

