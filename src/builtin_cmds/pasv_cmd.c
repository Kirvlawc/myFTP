/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** pasv_cmd
*/

#include "myftp.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

static void assign_message(char *message, connex_t *user_connex);

static void get_ip(int ip[4]);

static void generate_port(pass_port_t *port);

static int contains_errors(int fd, connex_t *user_connex, command_t *cmd);

void pasv_cmd(int fd, command_t *cmd)
{
    char message[256];
    connex_t *user_connex = find_in_list(fd);

    if (contains_errors(fd, user_connex, cmd))
        return;
    else {
        if (user_connex->pasv_fd != -1) close(user_connex->pasv_fd);
        assign_message(message, user_connex);
        user_connex->type = PASSIVE;
        user_connex->response = message;
        send_all(fd, user_connex->response, strlen(user_connex->response));
    }
}

static void assign_message(char *message, connex_t *user_connex)
{
    int ip_addr[4];
    int listen_fd;
    char port_str[10];
    pass_port_t port;

    get_ip(ip_addr);
    generate_port(&port);
    sprintf(port_str, "%d", 256 * port.port1 + port.port2);
    listen_fd = get_sock(port_str);
    listen_for_conn(listen_fd);
    user_connex->pasv_fd = listen_fd;
    sprintf(message, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d).\n",
        ip_addr[0], ip_addr[1], ip_addr[2], ip_addr[3], port.port1,
        port.port2);
}

static void get_ip(int ip[4])
{
    char host_buffer[256];
    char *ip_buffer;
    struct hostent *host_entry;
    int hostname;

    if (gethostname(host_buffer, sizeof(host_buffer)) == -1) {
        perror("gethostname");
        exit(84);
    }
    if ((host_entry = gethostbyname(host_buffer)) == NULL) {
        perror("gethostbyname");
        exit(84);
    }
    if ((ip_buffer = inet_ntoa(*((struct in_addr *)host_entry->h_addr_list[0])))
        == NULL) {
        perror("inet_ntoa");
        exit(84);
    }
    sscanf(ip_buffer, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);
}

static void generate_port(pass_port_t *port)
{
    srand(time(NULL));
    port->port1 = 4 + (rand() % 188);
    port->port2 = rand() % 0xff;
}

static int contains_errors(int fd, connex_t *user_connex, command_t *cmd)
{
    if (!user_connex->logged_in) {
        user_connex->response = "530 Not logged in.\n";
        send_all(fd, user_connex->response, strlen(user_connex->response));
        return (1);
    }
    if (strcmp("", cmd->args) != 0) {
        user_connex->response =
            "501 Syntax error in parameters or arguments.\n";
        send_all(fd, user_connex->response, strlen(user_connex->response));
        return (1);
    }
    return (0);
}