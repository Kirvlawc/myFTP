/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** get_socket
*/

#include "myftp.h"
#include <netdb.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

static void config_sock(int sfd, struct addrinfo *serv_info);

int get_sock(char *port)
{
    int sfd;
    int status;
    struct addrinfo hints;
    struct addrinfo *serv_info = NULL;

    memset(&hints, 0, sizeof(hints));
    hints.ai_addr = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((status = getaddrinfo(NULL, port, &hints, &serv_info)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(84);
    }
    if ((sfd = socket(serv_info->ai_family, serv_info->ai_socktype,
        serv_info->ai_protocol)) == -1) {
        perror("socket");
        exit(84);
    }
    config_sock(sfd, serv_info);
    freeaddrinfo(serv_info);
    return (sfd);
}

static void config_sock(int sfd, struct addrinfo *serv_info)
{
    int yes = 1;

    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("setsockopt");
        exit(84);
    }
    if (bind(sfd, serv_info->ai_addr, serv_info->ai_addrlen) == -1) {
        perror("bind");
        exit(84);
    }
}