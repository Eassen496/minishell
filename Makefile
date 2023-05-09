SRC		=  minishell.c get_next_line.c
OBJS	= ${SRC:.c=.o}
CC		= gcc
CFLAGS	= -Wall -Wextra -Werror
NAME	= minishell
RM		= rm -rf

.c.o	:
	${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}	: ${OBJS}
	gcc ${CFLAGS} -lreadline ${OBJS} -o ${NAME}

all		: ${NAME}

clean	:
	${RM} ${OBJS}

fclean	: clean
	${RM} ${NAME}

re		: fclean all

.PHONY	:	fclean clean re all
