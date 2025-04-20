NAME = minishell

C_Compiler = cc
CFLAGS = -Wall -Wextra -Werror

READLINEFLAG = -lreadline

READLINE_L = $(shell brew --prefix readline)/lib
READLINE_I = $(shell brew --prefix readline)/include

Funs = src/main.c library/libft/ft_atoi.c library/libft/ft_atol.c library/libft/ft_dstrlen.c library/libft/ft_isalnum.c library/libft/ft_isalpha.c library/libft/ft_isdigit.c library/libft/ft_putchar_fd.c library/libft/ft_putstr_fd.c library/libft/ft_split.c library/libft/ft_strchr.c library/libft/ft_strcmp.c library/libft/ft_strdup.c library/libft/ft_strjoin.c library/libft/ft_strlen.c library/libft/ft_strncmp.c library/libft/ft_substr.c

OFuns = $(Funs:.c=.o)

all: print_header $(NAME)

$(NAME): $(OFuns)
	$(C_Compiler) $(CFLAGS)  $^ -o $@ -L $(READLINE_L) $(READLINEFLAG)

%.o: %.c include/minishell.h include/parser.h include/executor.h
	$(C_Compiler) $(CFLAGS) -I $(READLINE_I) -c $< -o $@

clean:
	$(RM) $(OFuns)

fclean: clean
	$(RM) $(NAME)

re: fclean all

print_header:

	@echo "███╗░░░███╗██╗███╗░░██╗██╗░██████╗██╗░░██╗███████╗██╗░░░░░██╗░░░░░"
	@echo "████╗░████║██║████╗░██║██║██╔════╝██║░░██║██╔════╝██║░░░░░██║░░░░░"
	@echo "██╔████╔██║██║██╔██╗██║██║╚█████╗░███████║█████╗░░██║░░░░░██║░░░░░"
	@echo "██║╚██╔╝██║██║██║╚████║██║░╚═══██╗██╔══██║██╔══╝░░██║░░░░░██║░░░░░"
	@echo "██║░╚═╝░██║██║██║░╚███║██║██████╔╝██║░░██║███████╗███████╗███████╗"
	@echo "╚═╝░░░░░╚═╝╚═╝╚═╝░░╚══╝╚═╝╚═════╝░╚═╝░░╚═╝╚══════╝╚══════╝╚══════╝"
	@echo "\033[38;2;255;105;97m"
	@echo "                       by: soukaina and mohamed amin                   "
	@echo "\033[0m"


.PHONY: clean
