/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** listfiles_act
*/

#include "myftp.h"
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

static int try_connex(connex_t *user_connex, int *dest_fd);

void listfiles_act(char *path, connex_t *user_connex, int client_fd)
{
    int dest_fd;

    user_connex->response = "150 Here comes the directory listing.\n";
    send_all(client_fd, user_connex->response, strlen(user_connex->response));
    if (!try_connex(user_connex, &dest_fd))
        connex_open_failure(user_connex);
    user_connex->response = "226 Directory send OK.\n";
    send_all(client_fd, user_connex->response, strlen(user_connex->response));
    send_list(dest_fd, path);
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