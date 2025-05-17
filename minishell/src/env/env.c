

#include "../../include/executor.h"

char	*extract_data(char *line, int flag)
{
	size_t	i;

	i = 0;
	while (line[i] && line[i] != '=')
		i++;
	if (flag == 1)
	{
		if (!line[i])
			return (NULL);
		return (ft_substr(line, ++i, ft_strlen(line))); // leaks
	}
	return (ft_substr(line, 0, i)); // leaks
}

t_env	*append_node(t_env *head, char *key, char *val)
{
	t_env *tmp;
	t_env *node;

	node = malloc(sizeof(t_env));
	if (!node)
		exit(1); // free
	node->key = key;
	node->value = val;
	node->next = NULL;
	node->prev = NULL;
	if (!head)
		return (node);
	tmp = head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
	node->prev = tmp;
	return (head);
}

t_env	*search_for_defaults(t_env *envh, char *key)
{
	while (envh)
	{
		if (!ft_strcmp(envh->key, key))
			return (envh);
		envh = envh->next;
	}
	return (NULL);
}

int	handle_defaults(t_env **envh)
{
	char	*vals[4];
	char	*keys[4];
	size_t	len;

	len = 0;
	keys[0] = ft_strdup("OLDPWD");
	vals[0] = NULL;
	keys[1] =ft_strdup("PWD");
	vals[1] = getcwd(NULL, 0);
	if (!vals[1])
	{
		printf("bash: cd: getcwd Failed\n");
		return (1);
	} //leaks
	keys[2] = ft_strdup("SHLVL");
	vals[2] = ft_strdup("1");
	keys[3] = ft_strdup("PATH");
	vals[3] = ft_strdup("/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.");
	while (len < 4)
	{
		if(!search_for_defaults(*envh, keys[len]))
			*envh = append_node(*envh, keys[len], vals[len]);
		len++;
	}
	return (0);
}

char	*handle_shlvl(char *val)
{// TODO: Test it more; you are NOT fully understand how it works 
	long long		raw_input;
	unsigned int	wrapped;
	int				parsed;

	raw_input = atoll(val); //TODO : make ur atoll
	if (raw_input == 999)
		return (ft_strdup(""));
	if (raw_input < INT_MIN || raw_input > INT_MAX)
	{
		wrapped = (unsigned int)raw_input;
		parsed = wrapped +1;
	}
	else
		parsed = (int)raw_input +1;
	if (parsed < 0)
		parsed = 0;
	else if (parsed > 1000)
		parsed = 1;
	return (ft_strdup(ft_itoa(parsed)));
}

t_env	*fill_env(char **envp)
{
	size_t	i;
	t_env	*envh;
	char	*key;
	char	*val;

	envh = NULL;
	i = 0;
	if (envp && *envp)
	{
		while (envp[i])
		{
			val = extract_data(envp[i], 1);
			key =  extract_data(envp[i], 0);
			if (!ft_strcmp(key, "SHLVL"))
				val = handle_shlvl(val);
			envh = append_node(envh, key, val);
			i++;
		}
	}
	if (handle_defaults(&envh))
		return (NULL);
	return (envh);
}
