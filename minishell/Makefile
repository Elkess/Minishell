NAME = minishell

C_Compiler = cc
CFLAGS = -Iinclude #-Wall -Wextra -Werror

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
		src/parser/pars_err.c \
		src/parser/build_tree_utils.c \
		src/parser/build_tree.c \
		src/builtins/cd.c \
		src/builtins/echo.c \
		src/builtins/env.c \
		src/builtins/exit.c \
		src/builtins/export.c \
		src/builtins/pwd.c \
		src/builtins/unset.c \
		src/executor/execute_tree.c\
		src/executor/executor.c \
		src/executor/helper_funcs.c \
		src/env/env.c  \
		library/libft/ft_atoi.c       \
		library/libft/ft_isalnum.c    \
		library/libft/ft_itoa.c       \
		library/libft/ft_split.c      \
		library/libft/ft_strdup.c     \
		library/libft/ft_strncmp.c\
		library/libft/ft_atol.c       \
		library/libft/ft_isalpha.c    \
		library/libft/ft_putchar_fd.c \
		library/libft/ft_strchr.c     \
		library/libft/ft_strjoin.c    \
		library/libft/ft_substr.c\
		library/libft/ft_dstrlen.c    \
		library/libft/ft_isdigit.c    \
		library/libft/ft_putstr_fd.c  \
		library/libft/ft_strcmp.c     \
		library/libft/ft_strlen.c \

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
	
	@echo ""
	@echo ""
	@echo "\033[38;2;255;0;0m███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     "
	@echo "\033[38;2;204;0;0m████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     "
	@echo "\033[38;2;153;0;0m██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     "
	@echo "\033[38;2;102;0;0m██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     "
	@echo "\033[38;2;51;0;0m██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗"
	@echo "\033[38;2;0;0;0m╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝"
	@echo "\033[0m"
	@echo ""
	@echo "                by: soukaina and mohamed amin                   "
	@echo ""


.PHONY: clean
