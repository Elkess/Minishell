/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgmih <sgmih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:21:18 by sgmih             #+#    #+#             */
/*   Updated: 2025/04/20 09:17:39 by sgmih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parser.h"

int	g_signal;

void	ft_handle_signals(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = 1;
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	setup_signals(void)
{
	g_signal = 0;
	signal(SIGINT, ft_handle_signals);
	signal(SIGQUIT, SIG_IGN);
	rl_catch_signals = 0;
}

char	*ft_get_prompt(int exit_status)
{
	char	*prompt;

	prompt = readline("minishell$>$ ");
	if (prompt && prompt[0])
		add_history(prompt);
	if (!prompt)
	{
		printf("exit");
		exit(exit_status);
	}
	return (prompt);
}


char *preprocess_input(const char *line)
{
    int len = strlen(line);
    char *result = malloc(len * 3);
    if (!result)
	{
        perror("malloc failed");
        exit(0);
    }

    int j = 0;
    int i = 0;
    while (i < len) {
        char c = line[i];

		if (c == '|' || c == '<' || c == '>')
		{
            if (j > 0 && result[j - 1] != ' ') result[j++] = ' ';
            result[j++] = c;
            if (line[i + 1] != ' ') result[j++] = ' ';
        }
		else if (!isspace(c))
		{
            result[j++] = c;
        }
		else if (isspace(c) && (j > 0 && result[j - 1] != ' '))
		{
            result[j++] = ' ';
        }
        i++;
    }
    result[j] = '\0';
    return (result);
}

// Function to trim leading and trailing whitespace from a string
char *strtrim(const char *str) {
    if (!str) return NULL;

    // Find the first non-whitespace character
    while (isspace((unsigned char)*str)) str++;

    // Find the last non-whitespace character
    const char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Allocate and copy the trimmed string
    size_t len = end - str + 1;
    char *trimmed = malloc(len + 1);
    if (!trimmed) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    strncpy(trimmed, str, len);
    trimmed[len] = '\0';
    return trimmed;
}

// Function to create a new token
t_token *create_token(t_token_type type, const char *value) {
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    new_token->type = type;
    new_token->value = strdup(value); // Copy the value
    new_token->next = NULL;
    return new_token;
}

// Function to add a token to the end of the list
void add_token(t_token **head, t_token *new_token) {
    if (!*head) {
        *head = new_token;
    } else {
        t_token *current = *head;
        while (current->next) {
            current = current->next;
        }
        current->next = new_token;
    }
}

// Function to detect the token type based on the string
t_token_type detect_token_type(const char *str) {
    if (strcmp(str, "|") == 0) {
        return TOKEN_PIPE;
    } else if (strcmp(str, "&&") == 0) {
        return TOKEN_AND;
    } else if (strcmp(str, "||") == 0) {
        return TOKEN_OR;
    } else if (strcmp(str, "<") == 0) {
        return TOKEN_REDIR_IN;
    } else if (strcmp(str, ">") == 0) {
        return TOKEN_REDIR_OUT;
    } else if (strcmp(str, ">>") == 0) {
        return TOKEN_REDIR_APPEND;
    } else if (strcmp(str, "<<") == 0) {
        return TOKEN_REDIR_HEREDOC;
    } else {
        return TOKEN_WORD; // Default to word for commands/arguments
    }
}

// Tokenize the input string using ft_split and strtrim
t_token *tokenize_input(const char *line) {
    t_token *tokens = NULL;

    // Split the input line into parts using ft_split
    char **parts = ft_split((char *)line, ' '); // Split by spaces
    if (!parts) return NULL;

    // Iterate over the parts
    for (int i = 0; parts[i] != NULL; i++) {
        char *trimmed = strtrim(parts[i]); // Trim whitespace from each part
        if (*trimmed != '\0') { // Ignore empty tokens
            // Detect the token type and create a token
            t_token_type type = detect_token_type(trimmed);
            t_token *new_token = create_token(type, trimmed);
            // Add the token to the list
            add_token(&tokens, new_token);
        }
        free(trimmed); // Free the trimmed string
    }

    // Free the split parts
    for (int i = 0; parts[i] != NULL; i++) {
        free(parts[i]);
    }
    free(parts);

    return tokens;
}

// Function to free the token list
void free_tokens(t_token *tokens) {
    while (tokens) {
        t_token *temp = tokens;
        tokens = tokens->next;
        free(temp->value);
        free(temp);
    }
}


/************************************************************************************************/
// Function to get the token type as a string
const char *get_token_type_name(t_token_type type) {
    switch (type) {
        case TOKEN_WORD: return "TOKEN_WORD";
        case TOKEN_PIPE: return "TOKEN_PIPE";
        case TOKEN_AND: return "TOKEN_AND";
        case TOKEN_OR: return "TOKEN_OR";
        case TOKEN_REDIR_IN: return "TOKEN_REDIR_IN";
        case TOKEN_REDIR_OUT: return "TOKEN_REDIR_OUT";
        case TOKEN_REDIR_APPEND: return "TOKEN_REDIR_APPEND";
        case TOKEN_REDIR_HEREDOC: return "TOKEN_REDIR_HEREDOC";
        case TOKEN_GROUP: return "TOKEN_GROUP";
        default: return "UNKNOWN_TOKEN";
    }
}

// Function to print the tokens
void print_tokens(t_token *tokens) {
    t_token *current = tokens;
    while (current) {
        printf("%s(\"%s\")", get_token_type_name(current->type), current->value);
        if (current->next) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf("\n");
}


/************************************************************************************************/

void handle_input(const char *line)
{
    char *processed = preprocess_input(line);
    printf("Processed Input: \"%s\"\n", processed);

	t_token *tokens = tokenize_input(processed);

	// Print the tokens
    t_token *current = tokens;
    print_tokens(current);
	
	free_tokens(tokens);
    free(processed);
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	//t_tree	*tree;

	(void)ac;
	(void)av;
	(void)env;

	setup_signals();
	while (1)
	{
		line = ft_get_prompt(0);
		printf("[DEBUG] Line input: %s\n", line);

		handle_input(line);
		
		free(line);
		g_signal = 0;
	}
	return (0);
}
