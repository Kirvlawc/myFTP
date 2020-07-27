/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** listfile_pasv
*/

#include "myftp.h"
#include <string.h>

static int try_connex(connex_t *user_connex);

void listfiles_pasv(char *path, connex_t *user_connex, int client_fd)
{
    int dest_fd;

    user_connex->response = "150 Here comes the directory listing.\n";
    send_all(client_fd, user_connex->response, strlen(user_connex->response));
    dest_fd = try_connex(user_connex);
    user_connex->response = "226 Directory send OK.\n";
    send_all(client_fd, user_connex->response, strlen(user_connex->response));
    send_list(dest_fd, path);
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