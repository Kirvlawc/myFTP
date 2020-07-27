/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** accept_con
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

int accept_conn(int server_fd, struct sockaddr_storage *client_addr)
{
    int client_fd;
    socklen_t client_addr_size = sizeof(*client_addr);

    if ((client_fd = accept(server_fd, (struct sockaddr *)client_addr,
        &client_addr_size)) == -1) {
        perror("accept");
        exit(84);
    }
    return (client_fd);
}