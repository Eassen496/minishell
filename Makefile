SRC		= minishell.c get_next_line.c builtins.c builtins2.c builtins3.c command.c environment.c environment2.c handle_words.c historic.c parsing.c parsing2.c token.c tools.c tools2.c tools3.c
OBJS	= ${SRC:.c=.o}
CC		= gcc
CFLAGS	= -Wall -Wextra -Werror
NAME	= minishell
RM		= rm -rf

.c.o	:
	${CC} ${CFLAGS} -c $< -o ${<:.c=.o} -I ~/.brew/Cellar/readline/8.2.1/include

${NAME}	: ${OBJS}
	${CC} ${CFLAGS} -lreadline -L ~/.brew/Cellar/readline/8.2.1/lib ${OBJS} -o ${NAME}

all		: ${NAME}

clean	:
	${RM} ${OBJS}

fclean	: clean
	${RM} ${NAME}

re		: fclean all

.PHONY	:	fclean clean re all
