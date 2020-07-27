##
## EPITECH PROJECT, 2019
## NWP_myftp_2019
## File description:
## Makefile for FTP bootstrap 2019
##


SRC		=	./src/builtin_cmds/cdup_cmd.c		\
			./src/builtin_cmds/cwd_cmd.c		\
			./src/builtin_cmds/dele_cmd.c		\
			./src/builtin_cmds/help_cmd.c		\
			./src/builtin_cmds/list_cmd.c		\
			./src/builtin_cmds/listfiles_act.c	\
			./src/builtin_cmds/listfiles_pasv.c	\
			./src/builtin_cmds/noop_cmd.c		\
			./src/builtin_cmds/pass_cmd.c		\
			./src/builtin_cmds/pasv_cmd.c		\
			./src/builtin_cmds/port_cmd.c		\
			./src/builtin_cmds/pwd_cmd.c		\
			./src/builtin_cmds/quit_cmd.c		\
			./src/builtin_cmds/recvfile_act.c	\
			./src/builtin_cmds/recvfile_pasv.c	\
			./src/builtin_cmds/retr_cmd.c		\
			./src/builtin_cmds/sendfile_act.c	\
			./src/builtin_cmds/sendfile_pasv.c	\
			./src/builtin_cmds/stor_cmd.c		\
			./src/builtin_cmds/user_cmd.c		\
			./src/accept_conn.c					\
			./src/get_sock.c					\
			./src/list_manips.c					\
			./src/listen_for_conn.c				\
			./src/main.c						\
			./src/monitor_fds.c					\
			./src/recv_all.c					\
			./src/recv_file.c					\
			./src/resolve_path.c				\
			./src/respond.c						\
			./src/send_all.c					\
			./src/send_file.c					\
			./src/send_list.c					\
			./src/server.c						\
			./src/transfer_outcome.c			\

OBJ     =	$(SRC:.c=.o)

NAME	=	myftp

CFLAGS 	+= 	-Werror -Wextra

INCLUDE = -I ./include

%.o: %.c
	$(CC) -g $(CFLAGS) $(LDFLAGS) $(INCLUDE) -c $< -o $@

all:	$(NAME)

$(NAME):	$(OBJ)
	gcc $(OBJ) -o $(NAME)
	make clean

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re:	fclean all

.PHONY:
	all clean fclean re

.SILENT:
	all clean fclean re
