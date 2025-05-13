
#include "../../include/parser.h"

// Helper function to convert token type to string for readable output
static const char *get_token_type_name(t_token_type type)
{
    switch (type)
    {
        case TOKEN_WORD: return "WORD";
        case TOKEN_PIPE: return "PIPE";
        case TOKEN_AND: return "AND";
        case TOKEN_OR: return "OR";
        case TOKEN_SPACE: return "SPACE";
        case TOKEN_REDIR_IN: return "REDIR_IN";
        case TOKEN_REDIR_OUT: return "REDIR_OUT";
        case TOKEN_REDIR_APPEND: return "REDIR_APPEND";
        case TOKEN_REDIR_HEREDOC: return "REDIR_HEREDOC";
        case TOKEN_PAREN_OPEN: return "PAREN_OPEN";
        case TOKEN_PAREN_CLOSE: return "PAREN_CLOSE";
        case TOKEN_SINGL_AND: return "SINGL_AND";
        case TOKEN_SINGLE_QUOTE: return "SINGLE_QUOTE";
        case TOKEN_DOUBLE_QUOTE: return "DOUBLE_QUOTE";
        case TOKEN_DOLLAR: return "DOLLAR";
        case TOKEN_WILDCARD: return "WILDCARD";
        case TOKEN_SINGLE_QUOTED_WORD: return "SINGLE_QUOTED_WORD";
        case TOKEN_DOUBLE_QUOTED_WORD: return "DOUBLE_QUOTED_WORD";
        case TOKEN_HAS_QUOTED: return "TOKEN_HAS_QUOTED";
        case TOKEN_FILERED_OUT: return "FILERED_OUT";
        case TOKEN_FILERED_IN: return "FILERED_IN";
        case TOKEN_FILERED_APPEND: return "FILERED_APPEND";
        case TOKEN_FILERED_HEREDOC: return "FILERED_HEREDOC";
        default: return "UNKNOWN";
    }
}

// Function to print all tokens in the list, with spaces made visible as underscores
static void print_token_list(t_token *head)
{
    int i = 0;
    while (head)
    {
        printf("Token %d:", i++);
        printf("  Value   : ");
        if (head->value)
        {
            for (int j = 0; head->value[j]; j++)
            {
                if (head->value[j] == ' ')
                    printf("_");  // Replace space with underscore for visibility
                else
                    printf("%c", head->value[j]);
            }
        }
        else
            printf("NULL");
        printf("  Type    : %s", get_token_type_name(head->type));
        printf("  Priority: %d\n", head->priority);
        head = head->next;
    }
}

static void print_tool(t_tool tool)
{
    printf("t_tool struct:\n");
    printf("  paren   = %d\n", tool.paren);
    printf("  quoted  = %d\n", tool.quoted);
    printf("  anderr  = %d\n", tool.anderr);
    printf("  err     = %d\n", tool.err);
}


// Helper function to convert node type to string
static const char *node_type_to_str(t_node_type type) {
    switch (type) {
        case NODE_COMMAND: return "COMMAND";
        case NODE_PIPE: return "PIPE";
        case NODE_AND: return "AND";
        case NODE_OR: return "OR";
        default: return "UNKNOWN";
    }
}

// Helper function to convert redirection type to string
static const char *redir_type_to_str(t_redir_type type) {
    switch (type) {
        case REDIR_IN: return "<";
        case REDIR_OUT: return ">";
        case REDIR_APPEND: return ">>";
        case REDIR_HEREDOC: return "<<";
        default: return "NONE";
    }
}

// Function to print the tree with indentation
void print_tree(t_tree *tree, int level) {
    if (!tree) {
        printf("%*s[NULL]\n", level * 2, "");
        return;
    }

    // Print current node
    printf("%*sNode: %s\n", level * 2, "", node_type_to_str(tree->type));

    // Print command if present
    if (tree->cmd && tree->cmd[0]) {
        printf("%*sCommand: ", level * 2 + 2, "");
        for (int i = 0; tree->cmd[i]; i++) {
            printf("%s", tree->cmd[i]);
            if (tree->cmd[i + 1])
                printf(" ");
        }
        printf("\n");
    }

    // Print redirections if present
    t_redir *redir = tree->redirs;
    while (redir) {
        printf("%*sRedirection: %s %s\n", level * 2 + 2, "",
               redir_type_to_str(redir->type), redir->file ? redir->file : "(null)");
        redir = redir->next;
    }

    // Print file descriptors if set
    if (tree->fd[0] != -1 || tree->fd[1] != -1) {
        printf("%*sFDs: [%d, %d]\n", level * 2 + 2, "", tree->fd[0], tree->fd[1]);
    }

    // Print ambiguous flag if set
    if (tree->is_ambiguous) {
        printf("%*sAmbiguous: %d\n", level * 2 + 2, "", tree->is_ambiguous);
    }

    // Recursively print left and right children
    printf("%*sLeft:\n", level * 2 + 2, "");
    print_tree(tree->left, level + 1);
    printf("%*sRight:\n", level * 2 + 2, "");
    print_tree(tree->right, level + 1);
}

/******************************************** end print tokens *********************************************/


// Revised AST Tree Builder for minishell

// Function to create a tree node
t_tree *create_tree_node(t_node_type type, t_tool *tool)
{
    t_tree *node;

    node = (t_tree *) malloc(sizeof(t_tree));
    if (!node)
        return (NULL);
    add_to_grbg(&tool->grbg, node);
    node->type = type;
    node->cmd = NULL;
    node->redirs = NULL;
    node->fd[0] = -1;
    node->fd[1] = -1;
    node->is_ambiguous = 0;
    node->left = NULL;
    node->right = NULL;
    return (node);
}

// Function to create redirections with proper error handling
t_redir *create_redirection(t_token *redir_token, t_token *file_token) {
    t_redir *new_redir = malloc(sizeof(t_redir));
    if (!new_redir)
        return NULL;

    // Initialize redirection based on token type
    switch (redir_token->type) {
        case TOKEN_REDIR_IN:
            new_redir->type = REDIR_IN;
            break;
        case TOKEN_REDIR_OUT:
            new_redir->type = REDIR_OUT;
            break;
        case TOKEN_REDIR_APPEND:
            new_redir->type = REDIR_APPEND;
            break;
        case TOKEN_REDIR_HEREDOC:
            new_redir->type = REDIR_HEREDOC;
            break;
        default:
            new_redir->type = REDIR_NONE;
    }

    // Set file name and other initialization
    new_redir->file = strdup(file_token->value);
    new_redir->fd = -1;
    new_redir->flag = 0;
    new_redir->next = NULL;
    new_redir->index = 0;

    return new_redir;
}

// Function to add redirection to the list
void add_redirection(t_redir **head, t_redir *new_redir) {
    if (!*head) {
        *head = new_redir;
        return;
    }

    t_redir *current = *head;
    while (current->next)
        current = current->next;
    current->next = new_redir;
}

// Process redirections from token stream
t_redir *process_redirections(t_token **input, t_tool *tool)
{
    t_redir *redirections = NULL;

    while (*input) {
        // Skip spaces
        while (*input && (*input)->type == TOKEN_SPACE)
            *input = (*input)->next;
        
        // Check for redirection tokens
        if (*input && ((*input)->type == TOKEN_REDIR_IN || (*input)->type == TOKEN_REDIR_OUT ||
                       (*input)->type == TOKEN_REDIR_APPEND || (*input)->type == TOKEN_REDIR_HEREDOC))
        {
            t_token *redir_token = *input;
            *input = (*input)->next;
            
            // Skip spaces between redirection and filename
            while (*input && (*input)->type == TOKEN_SPACE)
                *input = (*input)->next;
            
            // Process the file token
            if (*input && ((*input)->type == TOKEN_FILERED_IN || (*input)->type == TOKEN_FILERED_OUT ||
                          (*input)->type == TOKEN_FILERED_APPEND || (*input)->type == TOKEN_FILERED_HEREDOC ||
                          (*input)->type == TOKEN_WORD))
            {
                t_redir *new_redir = create_redirection(redir_token, *input);
                if (new_redir)
                    add_redirection(&redirections, new_redir);
                *input = (*input)->next;
            }
            else
                break; // Error: missing file after redirection
        }
        else
            break; // No more redirections
    }
    return redirections;
}

// Parse a simple command (cmd + redirections)
t_tree *command_node(t_token **input, t_tool *tool)
{
    t_tree *node = NULL;
    t_token *start = *input;
    int word_count = 0;
    
    // Count words to allocate command array
    t_token *counter = *input;
    while (counter) {
        if (counter->type == TOKEN_WORD)
            word_count++;
        else if (counter->type == TOKEN_PIPE || counter->type == TOKEN_AND || 
                 counter->type == TOKEN_OR || counter->type == TOKEN_PAREN_CLOSE)
            break;
        else if (counter->type == TOKEN_REDIR_IN || counter->type == TOKEN_REDIR_OUT || 
                 counter->type == TOKEN_REDIR_APPEND || counter->type == TOKEN_REDIR_HEREDOC) {
            // Skip redirection and its filename
            counter = counter->next;
            while (counter && counter->type == TOKEN_SPACE)
                counter = counter->next;
            if (counter)
                counter = counter->next;
        }
        else if (counter->type != TOKEN_SPACE)
            break;
        if (counter)
            counter = counter->next;
    }
    
    // Create command node
    node = create_tree_node(NODE_COMMAND, tool);
    if (!node)
        return NULL;
    
    // Process all redirections first
    t_redir *redirs = process_redirections(input, tool);
    
    // Allocate and fill cmd array if there are words
    if (word_count > 0) {
        char **cmd = malloc((word_count + 1) * sizeof(char *));
        if (!cmd)
            return node;
        
        add_to_grbg(&tool->grbg, cmd);
        int i = 0;
        
        // Reset input to start position to collect words
        t_token *cmd_input = start;
        while (cmd_input && i < word_count) {
            // Process words only, skip redirections and their filenames
            if (cmd_input->type == TOKEN_WORD) {
                cmd[i] = strdup(cmd_input->value);
                add_to_grbg(&tool->grbg, cmd[i]);
                i++;
            }
            else if (cmd_input->type == TOKEN_REDIR_IN || cmd_input->type == TOKEN_REDIR_OUT || 
                    cmd_input->type == TOKEN_REDIR_APPEND || cmd_input->type == TOKEN_REDIR_HEREDOC) {
                // Skip redirection and filename
                cmd_input = cmd_input->next;
                while (cmd_input && cmd_input->type == TOKEN_SPACE)
                    cmd_input = cmd_input->next;
                if (cmd_input)
                    cmd_input = cmd_input->next;
                continue;
            }
            if (cmd_input)
                cmd_input = cmd_input->next;
        }
        cmd[i] = NULL;
        node->cmd = cmd;
    }
    
    // Attach redirection list to command node
    node->redirs = redirs;
    
    // Skip to the next token
    while (*input && (*input)->type == TOKEN_SPACE)
        *input = (*input)->next;
    
    return node;
}

// Parse a pipeline sequence
t_tree *pipe_sequence(t_token **input, t_tool *tool)
{
    t_tree *left;
    
    // Skip spaces
    while (*input && (*input)->type == TOKEN_SPACE)
        *input = (*input)->next;
    
    // Parse the first command in the pipeline
    left = command_node(input, tool);
    if (!left)
        return NULL;
    
    // Check for pipe operator
    while (*input && (*input)->type == TOKEN_PIPE) {
        t_tree *pipe_node = create_tree_node(NODE_PIPE, tool);
        if (!pipe_node)
            return left;
        
        // Move to the next token after pipe
        *input = (*input)->next;
        
        // Skip spaces
        while (*input && (*input)->type == TOKEN_SPACE)
            *input = (*input)->next;
        
        // Parse the right side of the pipe
        t_tree *right = command_node(input, tool);
        if (!right)
            return left;
        
        // Link the pipe node
        pipe_node->left = left;
        pipe_node->right = right;
        left = pipe_node;
    }
    
    return left;
}

// Parse a logical AND sequence
t_tree *and_sequence(t_token **input, t_tool *tool)
{
    t_tree *left = pipe_sequence(input, tool);
    if (!left)
        return NULL;
    
    // Check for AND operator
    while (*input && (*input)->type == TOKEN_AND) {
        t_tree *and_node = create_tree_node(NODE_AND, tool);
        if (!and_node)
            return left;
        
        // Move to the next token after AND
        *input = (*input)->next;
        
        // Skip spaces
        while (*input && (*input)->type == TOKEN_SPACE)
            *input = (*input)->next;
        
        // Parse the right side of AND
        t_tree *right = pipe_sequence(input, tool);
        if (!right)
            return left;
        
        // Link the AND node
        and_node->left = left;
        and_node->right = right;
        left = and_node;
    }
    
    return left;
}

// Parse a logical OR sequence
t_tree *or_sequence(t_token **input, t_tool *tool)
{
    t_tree *left = and_sequence(input, tool);
    if (!left)
        return NULL;
    
    // Check for OR operator
    while (*input && (*input)->type == TOKEN_OR) {
        t_tree *or_node = create_tree_node(NODE_OR, tool);
        if (!or_node)
            return left;
        
        // Move to the next token after OR
        *input = (*input)->next;
        
        // Skip spaces
        while (*input && (*input)->type == TOKEN_SPACE)
            *input = (*input)->next;
        
        // Parse the right side of OR
        t_tree *right = and_sequence(input, tool);
        if (!right)
            return left;
        
        // Link the OR node
        or_node->left = left;
        or_node->right = right;
        left = or_node;
    }
    
    return left;
}

// Main parsing function
t_tree *build_tree(t_token **input, t_tool *tool)
{
    // Starting with the lowest precedence operator: OR
    return or_sequence(input, tool);
}











//Function to create redirections
t_redir *create_redirection(t_token *redir_token, t_token *file_token) {
    t_redir *new_redir = malloc(sizeof(t_redir));
    if (!new_redir)
        return NULL;

    // Initialize redirection based on token type
    switch (redir_token->type) {
        case TOKEN_REDIR_IN:
            new_redir->type = REDIR_IN;
            break;
        case TOKEN_REDIR_OUT:
            new_redir->type = REDIR_OUT;
            break;
        case TOKEN_REDIR_APPEND:
            new_redir->type = REDIR_APPEND;
            break;
        case TOKEN_REDIR_HEREDOC:
            new_redir->type = REDIR_HEREDOC;
            break;
        default:
            new_redir->type = REDIR_NONE;
    }

    // Set file name and other initialization
    new_redir->file = strdup(file_token->value);
    new_redir->fd = -1;
    new_redir->flag = 0;
    new_redir->next = NULL;
    new_redir->index = 0;  // You might want to set a meaningful index

    return new_redir;
}

// Function to add redirection to the list
void add_redirection(t_redir **head, t_redir *new_redir) {
    if (!*head) {
        *head = new_redir;
        return;
    }

    t_redir *current = *head;
    while (current->next)
        current = current->next;
    current->next = new_redir;
}

// Creates a new t_tree node
t_tree  *create_tree_node(t_node_type type, t_tool *tool)
{
    t_tree *node;

    node = (t_tree *) malloc(sizeof(t_tree));
    if (!node)
        return (NULL);
    add_to_grbg(&tool->grbg, node);
    node->type = type;
    node->cmd = NULL;
    node->redirs = NULL;
    node->fd[0] = -1;
    node->fd[1] = -1;
    node->is_ambiguous = 0;
    node->left = NULL;
    node->right = NULL;
    return (node);
}

// t_redir *redir(t_token **input, t_tool *tool)
// {
//     t_redir *redir_node;
//     t_redir *redirections = NULL;

//     redir_node = NULL;

//     while(*input && (*input)->type == TOKEN_SPACE)
//         *input = (*input)->next;
//     while (*input && ((*input)->type == TOKEN_REDIR_IN || (*input)->type == TOKEN_REDIR_OUT || 
//             (*input)->type == TOKEN_REDIR_APPEND || (*input)->type == TOKEN_REDIR_HEREDOC))
//     {
//         // Create and add redirection
//         t_redir *new_redir = create_redirection(redir_node, *input);
//         add_redirection(&redirections, new_redir);

//         *input = (*input)->next;
//     }
// }


t_redir *redir(t_token **input, t_tool *tool)
{
    t_redir *redirections = NULL;

    while (*input && (*input)->type == TOKEN_SPACE)
        *input = (*input)->next;

    while (*input && ((*input)->type == TOKEN_REDIR_IN || (*input)->type == TOKEN_REDIR_OUT ||
                      (*input)->type == TOKEN_REDIR_APPEND || (*input)->type == TOKEN_REDIR_HEREDOC))
    {
        t_token *redir_token = *input;
        *input = (*input)->next;
        while (*input && (*input)->type == TOKEN_SPACE)
            *input = (*input)->next;
        if (*input && ((*input)->type == TOKEN_FILERED_IN || (*input)->type == TOKEN_FILERED_OUT ||
                       (*input)->type == TOKEN_FILERED_APPEND || (*input)->type == TOKEN_FILERED_HEREDOC ||
                       (*input)->type == TOKEN_WORD))
        {
            t_redir *new_redir = create_redirection(redir_token, *input);
            add_redirection(&redirections, new_redir);
            *input = (*input)->next;
        }
    }
    return redirections;
}

t_tree *command_unit(t_token **input, t_tool *tool)
{
    t_redir *redirs = NULL;
    t_tree *node;
    char **cmd = NULL;
    int word_count = 0;
    t_token *start = *input;

    // Skip leading spaces
    while (*input && (*input)->type == TOKEN_SPACE)
        *input = (*input)->next;

    // Collect redirections and words until a separator (pipe, AND, OR, etc.)
    while (*input && (*input)->type != TOKEN_PIPE && (*input)->type != TOKEN_AND &&
           (*input)->type != TOKEN_OR && (*input)->type != TOKEN_PAREN_CLOSE)
    {
        if ((*input)->type == TOKEN_REDIR_IN || (*input)->type == TOKEN_REDIR_OUT ||
            (*input)->type == TOKEN_REDIR_APPEND || (*input)->type == TOKEN_REDIR_HEREDOC)
        {
            t_token *redir_token = *input;
            *input = (*input)->next;
            // Skip spaces
            while (*input && (*input)->type == TOKEN_SPACE)
                *input = (*input)->next;
            if (*input && ((*input)->type == TOKEN_FILERED_IN || (*input)->type == TOKEN_FILERED_OUT ||
                           (*input)->type == TOKEN_FILERED_APPEND || (*input)->type == TOKEN_FILERED_HEREDOC ||
                           (*input)->type == TOKEN_WORD))
            {
                t_redir *new_redir = create_redirection(redir_token, *input);
                add_redirection(&redirs, new_redir);
                *input = (*input)->next;
            }
        }
        else if ((*input)->type == TOKEN_WORD)
        {
            // Count words to allocate cmd array later
            word_count++;
            *input = (*input)->next;
        }
        else
            *input = (*input)->next; // Skip spaces or other tokens
    }

    // Create command node if there are words
    if (word_count > 0)
    {
        node = create_tree_node(NODE_COMMAND, tool);
        cmd = malloc((word_count + 1) * sizeof(char *));
        add_to_grbg(&tool->grbg, cmd);
        int i = 0;
        *input = start;
        while (*input && i < word_count)
        {
            if ((*input)->type == TOKEN_WORD)
            {
                cmd[i] = strdup((*input)->value);
                add_to_grbg(&tool->grbg, cmd[i]);
                i++;
            }
            *input = (*input)->next;
        }
        cmd[word_count] = NULL;
        node->cmd = cmd;
        node->redirs = redirs;
    }
    else
    {
        node = create_tree_node(NODE_COMMAND, tool);
        node->redirs = redirs;
    }

    return node;
}


t_tree *pipe_unit(t_token **input, t_tool *tool)
{
    t_tree *left;
    t_tree *right;
    t_tree *operator = NULL;
        
    while (*input && (*input)->type == TOKEN_SPACE)
        *input = (*input)->next;
        
    left = command_unit(input, tool);

    // Skip spaces after command
    while (*input && (*input)->type == TOKEN_SPACE)
        *input = (*input)->next;

    while (*input && (*input)->type == TOKEN_PIPE)
    {
        operator = create_tree_node((*input)->type, tool);
        *input = (*input)->next;
        
        // Skip spaces after pipe
        while (*input && (*input)->type == TOKEN_SPACE)
            *input = (*input)->next;
        
        right = pipe_unit(input, tool);
        operator->left = left;
        operator->right = right;
        left = operator;

        // Skip spaces after command
        while (*input && (*input)->type == TOKEN_SPACE)
            *input = (*input)->next;
    }
    return (left);
}




t_tree *build_tree(t_token **input, t_tool *tool)
{
    t_tree *left;
    t_tree *right;
    t_tree *operator = NULL;
    
    // Process pipeline first
    left = pipe_unit(input, tool);
    // Skip spaces
    while (*input && (*input)->type == TOKEN_SPACE)
        *input = (*input)->next;

    // Process logical operators (AND, OR)
    while (*input && ((*input)->type == TOKEN_AND || (*input)->type == TOKEN_OR))
    {
        operator = create_tree_node((*input)->type, tool);
        *input = (*input)->next;

        // Skip spaces
        while (*input && (*input)->type == TOKEN_SPACE)
            *input = (*input)->next;
            
        right = pipe_unit(input, tool);
        // Connect operator node
        operator->right = right;
        operator->left = left;
        left = operator;

        // Skip spaces
        while (*input && (*input)->type == TOKEN_SPACE)
            *input = (*input)->next;
    }
    if (*input && (*input)->type == TOKEN_PAREN_CLOSE)
    {
        *input = (*input)->next;
        if (*input && (*input)->type == TOKEN_SPACE)
            *input = (*input)->next;
    }
    if (operator)
        return (operator);
    return (left);
}


t_tree	*parsing_input(char *line, t_tool *tool)
{
	t_token	*token;
    t_tree	*tree;
    int i;
    
    tool->quoted = 0;
    tool->anderr = 0;
    tool->paren = 0;

    i = 0;
    while (line && line[i])
    {
        hundel_quotes_paren(tool, line[i]);
        i++;
    }
	
	token = tokens_lst(line, tool);
	print_token_list(token);

    //tree = build_tree(&token, tool);
    //print_tree(tree, 0);
	// print_tool(*tool);

    // Build AST
    tree = build_tree(&token, tool);
    print_tree(tree, 1);
	return (NULL);
}