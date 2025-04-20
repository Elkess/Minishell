NAME = minishell

C_Compiler = cc
CFLAGS = -Wall -Wextra -Werror

READLINEFLAG = -lreadline

READLINE_L = $(shell brew --prefix readline)/lib
READLINE_I = $(shell brew --prefix readline)/include

Funs = src/main.c library/libft/ft_split.c library/libft/ft_strlen.c

OFuns = $(Funs:.c=.o)

all: print_header $(NAME)

$(NAME): $(OFuns)
	$(C_Compiler) $(CFLAGS)  $^ -o $@ -L $(READLINE_L) $(READLINEFLAG)

%.o: %.c include/minishell.h include/parser.h
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
