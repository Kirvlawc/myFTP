/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** Command for specifying user
*/

#include "myftp.h"
#include <stdio.h>
#include <string.h>

static void contains_errors(int fd, connex_t *user_connex, command_t *cmd);

void user_cmd(int fd, command_t *cmd)
{
    connex_t *user_connex = find_in_list(fd);

    if (user_connex && user_connex->logged_in == 1) {
        user_connex->u_name = cmd->args;
        user_connex->response = "230 User logged in, proceed.\n";
        send_all(fd, user_connex->response, strlen(user_connex->response));
    } else if (user_connex && strcmp("", cmd->args) != 0) {
        user_connex->u_name = cmd->args;
        user_connex->response = "331 User name okay, need password.\n";
        send_all(fd, user_connex->response, strlen(user_connex->response));
    } else contains_errors(fd, user_connex, cmd);
}

static void contains_errors(int fd, connex_t *user_connex, command_t *cmd)
{
    if (strcmp("", cmd->args) == 0) {
        user_connex->response = "530 Not logged in.\n";
        send_all(fd, user_connex->response, strlen(user_connex->response));
        return;
    }
}