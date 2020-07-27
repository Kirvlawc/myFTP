/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** recvfile_pasv
*/

#include "myftp.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

static void file_failure(connex_t *user_connex);

static int try_connex(connex_t *user_connex);

void recvfile_pasv(char *filename, connex_t *user_connex, int client_fd)
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
        src_fd = try_connex(user_connex);
        recv_file(src_fd, dest_fd, user_connex);
        user_connex->response = "226 Closing data connection. "
            "Requested file action successful.\n";
        send_all(client_fd, user_connex->response,
            strlen(user_connex->response));
    } else file_failure(user_connex);
}

static int try_connex(connex_t *user_connex)
{
    int data_fd;
    struct sockaddr_storage client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

    if ((data_fd = accept(user_connex->pasv_fd, (struct sockaddr *)&client_addr,
        &client_addr_size)) == -1)
        connex_open_failure(user_connex);
    return (data_fd);
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