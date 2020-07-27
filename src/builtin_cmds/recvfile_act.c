/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** recvfile_act
*/

#include "myftp.h"

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/types.h>

static void file_failure(connex_t *user_connex);

static int try_connex(connex_t *user_connex, int *dest_fd);

void recvfile_act(char *filename, connex_t *user_connex, int client_fd)
{
    int dest_fd;
    int flags = O_WRONLY | O_CREAT | O_TRUNC;
    int src_fd;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;

    if ((dest_fd = open(filename, flags, mode)) != -1) {
        user_connex->response =
            "150 File status okay; about to open data connection.\n";
        send_all(client_fd, user_connex->response,
            strlen(user_connex->response));
        if (!try_connex(user_connex, &src_fd))
            connex_open_failure(user_connex);
        recv_file(src_fd, dest_fd, user_connex);
        user_connex->response = "226 Closing data connection. "
            "Requested file action successful.\n";
        send_all(client_fd, user_connex->response,
            strlen(user_connex->response));
    } else file_failure(user_connex);
}

static int try_connex(connex_t *user_connex, int *src_fd)
{
    int sfd;
    struct addrinfo hints;
    struct addrinfo *serv_info = NULL;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(user_connex->act_ip, user_connex->act_port,
        &hints, &serv_info) != 0)
        return (0);
    if ((sfd = socket(serv_info->ai_family, serv_info->ai_socktype,
        serv_info->ai_protocol)) == -1)
        return (0);
    if (connect(sfd, serv_info->ai_addr, serv_info->ai_addrlen) != 0)
        return (0);
    *src_fd = sfd;
    return (1);
}

static void file_failure(connex_t *user_connex)
{
    if (errno == ENOSPC || errno == EFBIG) {
        user_connex->response = "452 Requested action not taken.\n";
        send_all(user_connex->sock_fd, user_connex->response,
            strlen(user_connex->response));
    }
    if (errno == EINTR || errno == EMFILE || errno == ENOMEM ||
        errno == ETXTBSY) {
        user_connex->response = "450 Requested file action not taken.\n";
        send_all(user_connex->sock_fd, user_connex->response,
            strlen(user_connex->response));
    }
    if (errno == EISDIR || errno == ENAMETOOLONG) {
        user_connex->response = "553 Requested action not taken.\n";
        send_all(user_connex->sock_fd, user_connex->response,
            strlen(user_connex->response));
    }
    exit(84);
}