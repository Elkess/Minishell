// int	g_signal = 0;

// void	ft_handle_signals(int sig)
// {
// 	if (sig == SIGINT)
// 	{
// 		g_signal = 1;
// 		printf("\n");
// 		rl_replace_line("", 0);
// 		rl_on_new_line();
// 		rl_redisplay();
// 	}
// }

// void	setup_signals(void)
// {
// 	signal(SIGINT, ft_handle_signals);
// 	signal(SIGQUIT, SIG_IGN);
// 	//rl_catch_signals = 0;
// }

// char	*ft_get_prompt(int exit_status)
// {
// 	char	*prompt;

// 	prompt = readline("minishell$> ");
// 	if (prompt && prompt[0])
// 		add_history(prompt);
// 	if (!prompt)
// 	{
// 		printf("exit\n");
// 		exit(exit_status);
// 	}
// 	return (prompt);
// }

// int	main(int ac, char **av, char **env)
// {
// 	char	*line;
    
// 	(void)ac;
// 	(void)av;
// 	(void)env;

// 	//setup_signals();
// 	while (1)
// 	{
// 		line = ft_get_prompt(0);
// 		printf("Line input: %s\n", line);

// 		// handle_input(line);
		
// 		//free(line);
// 		g_signal = 0;
// 	}
// 	return (0);
// }

