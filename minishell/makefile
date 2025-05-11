NAME = minishell

C_Compiler = cc
CFLAGS = #-Wall -Wextra -Werror

READLINEFLAG = -lreadline

READLINE_L = $(shell brew --prefix readline)/lib
READLINE_I = $(shell brew --prefix readline)/include

Funs =	src/main.c \
		src/parser/parser.c \
		src/parser/token_lst.c  \
		src/parser/token_lst_utils.c  \
		src/parser/token_funs_help.c \
		src/parser/lst_helper.c  \
		src/parser/ft_garbage_collect.c \
		src/parser/pars_funs_help.c \
		src/parser/pars_err.c

OFuns = $(Funs:.c=.o)

all: print_header $(NAME)

$(NAME): $(OFuns)
	$(C_Compiler) $(CFLAGS)  $^ -o $@ -L $(READLINE_L) $(READLINEFLAG)
	@echo "\\033[0;32m ${NAME} created \\033[0m"

%.o: %.c include/minishell.h include/parser.h include/executor.h
	$(C_Compiler) $(CFLAGS) -I $(READLINE_I) -c $< -o $@

clean:
	@$(RM) $(OFuns)
	@echo "\\033[0;31m Object files ${NAME} removed \\033[0m"

fclean: clean
	@$(RM) $(NAME)
	@echo "\\033[0;31m ${NAME} removed \\033[0m"

re: fclean all

print_header:

	@echo "\\033[38;2;52;52;52m███╗   ███╗██╗███╗   ██╗██╗\\033[38;2;255;105;97m███████╗██╗  ██╗███████╗██╗     ██╗     "
	@echo "\\033[38;2;52;52;52m████╗ ████║██║████╗  ██║██║\\033[38;2;255;105;97m██╔════╝██║  ██║██╔════╝██║     ██║     "
	@echo "\\033[38;2;52;52;52m██╔████╔██║██║██╔██╗ ██║██║\\033[38;2;255;105;97m███████╗███████║█████╗  ██║     ██║     "
	@echo "\\033[38;2;52;52;52m██║╚██╔╝██║██║██║╚██╗██║██║\\033[38;2;255;105;97m╚════██║██╔══██║██╔══╝  ██║     ██║     "
	@echo "\\033[38;2;52;52;52m██║ ╚═╝ ██║██║██║ ╚████║██║\\033[38;2;255;105;97m███████║██║  ██║███████╗███████╗███████╗"
	@echo "\\033[38;2;52;52;52m╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝\\033[38;2;255;105;97m╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝"
	@echo "\033[0m"

	@echo "                by: soukaina and mohamed amin                   "


.PHONY: clean
