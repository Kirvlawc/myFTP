/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** sendfile_act
*/

#include "myftp.h"
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static void file_failure(connex_t *user_connex);

static int try_connex(connex_t *user_connex, int *dest_fd);

void sendfile_act(char *filename, connex_t *user_connex, int client_fd)
{
    int dest_fd;
    int src_fd;
    struct stat stat_buf;

    if ((src_fd = open(filename, O_RDONLY)) != -1) {
        fstat(src_fd, &stat_buf);
        user_connex->response =
            "150 File status okay; about to open data connection.\n";
        send_all(client_fd, user_connex->response,
            strlen(user_connex->response));
        if (!try_connex(user_connex, &dest_fd))
            connex_open_failure(user_connex);
        send_file(src_fd, dest_fd, stat_buf.st_size, user_connex);
        user_connex->response = "226 Closing data connection. "
            "Requested file action successful.\n";
        send_all(client_fd, user_connex->response,
            strlen(user_connex->response));
    } else file_failure(user_connex);
}

static int try_connex(connex_t *user_connex, int *dest_fd)
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
    *dest_fd = sfd;
    return (1);
}

static void file_failure(connex_t *user_connex)
{
    if (errno == EINTR || errno == EMFILE || errno == ENOMEM ||
        errno == ETXTBSY) {
        user_connex->response = "450 Requested file action not taken.\n";
        send_all(user_connex->sock_fd, user_connex->response,
            strlen(user_connex->response));
    } else {
        user_connex->response = "550 Requested action not taken."
            " File unavailable\n";
        send_all(user_connex->sock_fd, user_connex->response,
            strlen(user_connex->response));
    }
    exit(84);
}