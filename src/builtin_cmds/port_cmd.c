/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** port_cmd
*/

#include "myftp.h"
#include <stdio.h>
#include <string.h>

static int contains_errors(int fd, connex_t *user_connex, command_t *cmd);

static int is_valid_addr(connex_t *user_connex, command_t *cmd);

void port_cmd(int fd, command_t *cmd)
{
    connex_t *user_connex = find_in_list(fd);

    if (contains_errors(fd, user_connex, cmd))
        return;
    user_connex->type = ACTIVE;
    user_connex->response = "200 Command okay.\n";
    send_all(fd, user_connex->response, strlen(user_connex->response));
}

static int contains_errors(int fd, connex_t *user_connex, command_t *cmd)
{
    if (!user_connex->logged_in) {
        user_connex->response = "530 Not logged in.\n";
        send_all(fd, user_connex->response, strlen(user_connex->response));
        return (1);
    }
    if (strcmp("", cmd->args) == 0 || !is_valid_addr(user_connex, cmd)) {
        user_connex->response =
            "501 Syntax error in parameters or arguments.\n";
        send_all(fd, user_connex->response, strlen(user_connex->response));
        return (1);
    }
    return (0);
}

static int is_valid_addr(connex_t *user_connex, command_t *cmd)
{
    char ip_buff[20];
    char port_buff[20];
    int ip[4] = {-1, -1, -1, -1};
    int port[2] = {-1, -1};
    int ret_val;

    ret_val = sscanf(cmd->args, "%d,%d,%d,%d,%d,%d", &ip[0], &ip[1], &ip[2],
        &ip[3], &port[0], &port[1]);
    if (ret_val != 6) return (0);
    for (int i = 0; i < 4; i++) {
        if (ip[i] < 0 || ip[i] > 255) return (0);
    }
    if ((port[0] * 256 + port[1]) < 0 || (port[0] * 256 + port[1]) > 65535)
        return (0);
    sprintf(ip_buff, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    sprintf(port_buff, "%d", (port[0] * 256 + port[1]));
    user_connex->act_ip = strdup(ip_buff);
    user_connex->act_port = strdup(port_buff);
    return (1);
}