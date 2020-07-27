/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** main
*/

#include "myftp.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static void check_for_update(int fds[2], int *fd_max, fd_set *read_fds,
    fd_set *master);

static void handle_event(int fd, fd_set *readfds);

void launch_server(char *port)
{
    fd_set master;
    fd_set read_fds;
    int i = 0;
    int fd_max;
    int listen_fd = get_sock(port);
    int fds[2];

    listen_for_conn(listen_fd);
    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    FD_SET(listen_fd, &master);
    fd_max = listen_fd;
    fds[1] = listen_fd;
    while (1) {
        read_fds = master;
        monitor_fds(fd_max, &read_fds, &master);
        for (int i = 0; i <= fd_max; i++) {
            fds[0] = i;
            check_for_update(fds, &fd_max, &read_fds, &master);
        }
    }
}

static void check_for_update(int fds[2], int *fd_max, fd_set *read_fds,
    fd_set *master)
{
    int client_fd;
    struct sockaddr_storage client_addr;

    if (FD_ISSET(fds[0], read_fds)) {
        if (fds[0] == fds[1]) {
            client_fd = accept_conn(fds[1], &client_addr);
            FD_SET(client_fd, master);
            add_to_list(client_fd);
            *fd_max = (client_fd > *fd_max) ? client_fd : *fd_max;
            send_all(client_fd, "220 Service ready for new user.\n", 32);
        } else handle_event(fds[0], master);
    }
}

static void handle_event(int fd, fd_set *readfds)
{
    char *input = recv_all(fd);
    command_t cmd_in;
    int i = 0;
    int j = 0;

    if (input != NULL) {
        while (input[i] != ' ' && input[i] != '\0' && j < 5) {
            cmd_in.cmd[j++] = input[i++];
        }
        while (input[i] == ' ' || input[i] == '\t') i++;
        cmd_in.cmd[j] = '\0';
        cmd_in.args = &input[i];
        respond(fd, &cmd_in);
    } else {
        close(fd);
        FD_CLR(fd, readfds);
        delete_from_list(fd);
    }
}