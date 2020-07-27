/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** myftp
*/

#ifndef MYFTP_H_
#define MYFTP_H_

#include <dirent.h>
#include <stddef.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>

typedef enum trans_type {
    ACTIVE,
    PASSIVE,
    NONE
} trans_type_t;

typedef struct command {
    char cmd[5];
    char *args;
} command_t;

typedef struct connex {
    int sock_fd;
    int pasv_fd;
    int logged_in;
    char *u_name;
    char *response;
    char *c_dir_path;
    char *act_ip;
    char *act_port;
    DIR *c_dir;
    trans_type_t type;
    struct connex *next;
} connex_t;

typedef struct pass_port
{
    int port1;
    int port2;
} pass_port_t;

static connex_t *connect_list = NULL;

typedef enum cmds {
    USER,
    PASS,
    CWD,
    CDUP,
    QUIT,
    DELE,
    PWD,
    PASV,
    PORT,
    HELP,
    NOOP,
    RETR,
    STOR,
    LIST
} cmds_t;

static const char *CMDS_STR[] = { "USER", "PASS", "CWD", "CDUP", "QUIT", "DELE",
    "PWD", "PASV", "PORT", "HELP", "NOOP", "RETR", "STOR", "LIST"};

static const char *HELP_MSGS[] = {
    "214 USER <SP> <username> <CRLF>    : Specify user for authentication\n",
    "214 PASS <SP> <password> <CRLF>    :"
        " Specify password for authentication\n",
    "214 CWD  <SP> <pathname> <CRLF>    : Change working directory\n",
    "214 CDUP <CRLF>    : Change working directory to parent directory\n",
    "214 QUIT <CRLF>    : Disconnection\n",
    "214 DELE <SP> <pathname> <CRLF>    : Delete file on the server\n",
    "214 PWD  <CRLF>    : Print working directory\n",
    "214 PASV <CRLF>    : Enable \"passive\" mode for data transfer\n",
    "214 PORT <SP> <host-port> <CRLF>   :"
        " Enable \"active\" mode for data transfer\n",
    "214 HELP [<SP> <string>] <CRLF>    : List available commands\n",
    "214 NOOP <CRLF>    : Do nothing\n",
    "214 RETR <SP> <pathname> <CRLF>    :"
        " Download file from server to client\n",
    "214 STOR <SP> <pathname> <CRLF>    : Upload file from client to server\n",
    "214 LIST [<SP> <pathname>] <CRLF>  :"
        " List files in the current working directory\n"
};

static const int MAX_PENDING = 10;

static const int NUM_CMDS = 14;

extern char *ROOT_DIR;

extern char **ENV_LIST;

int accept_conn(int server_fd, struct sockaddr_storage *client_addr);

void add_to_list(int fd);

void cdup_cmd(int fd, command_t *cmd);

void connex_close_error(connex_t *user_connex);

void connex_open_failure(connex_t *user_connex);

void cwd_cmd(int fd, command_t *cmd);

void dele_cmd(int fd, command_t *cmd);

void delete_from_list(int fd);

connex_t *find_in_list(int fd);

int get_sock(char *port);

void help_cmd(int fd, command_t *cmd);

void launch_server(char *port);

void listen_for_conn(int listen_fd);

void list_cmd(int fd, command_t *cmd);

void listfiles_act(char *path, connex_t *user_connex, int client_fd);

void listfiles_pasv(char *filename, connex_t *user_connex, int client_fd);

void monitor_fds(int fd_max, fd_set *read_fds, fd_set *master);

void noop_cmd(int fd, command_t *cmd);

void pass_cmd(int fd, command_t *cmd);

void pasv_cmd(int fd, command_t *cmd);

void port_cmd(int fd, command_t *cmd);

void pwd_cmd(int fd, command_t *cmd);

void quit_cmd(int fd, command_t *cmd);

void read_failure(connex_t *user_connex);

char *recv_all(int client_fd);

void recv_file(int src_fd, int dest_fd, connex_t *user_connex);

void recvfile_act(char *filename, connex_t *user_connex, int client_fd);

void recvfile_pasv(char *filename, connex_t *user_connex, int client_fd);

void resolve_path(char *path, connex_t *user_connex, char *new_path,
    int cdup);

void retr_cmd(int fd, command_t *cmd);

void respond(int fd, command_t *cmd);

void user_cmd(int fd, command_t *cmd);

void send_all(int client_fd, const char *buffer, int len);

void send_file(int src_fd, int dest_fd, size_t size, connex_t *user_connex);

void sendfile_act(char *filename, connex_t *user_connex, int client_fd);

void sendfile_pasv(char *filename, connex_t *user_connex, int client_fd);

void send_list(int dest_fd, char *path);

void stor_cmd(int fd, command_t *cmd);

void write_failure(connex_t *user_connex);

static void (*cmd_funcs[14])(int fd, command_t *cmd) = {
    &user_cmd,
    &pass_cmd,
    &cwd_cmd,
    &cdup_cmd,
    &quit_cmd,
    &dele_cmd,
    &pwd_cmd,
    &pasv_cmd,
    &port_cmd,
    &help_cmd,
    &noop_cmd,
    &retr_cmd,
    &stor_cmd,
    &list_cmd
};

#endif /* !MYFTP_H_ */
