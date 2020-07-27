/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** pass_cmd
*/

#include "myftp.h"
#include <stdio.h>
#include <string.h>

static void contains_errors(int fd, connex_t *user_connex, command_t *cmd);

void pass_cmd(int fd, command_t *cmd)
{
    connex_t *user_connex = find_in_list(fd);

    if (user_connex && strcmp("Anonymous", user_connex->u_name) == 0 &&
        strcmp("", cmd->args) == 0) {
        user_connex->logged_in = 1;
        user_connex->response = "230 User logged in, proceed.\n";
        send_all(fd, user_connex->response, strlen(user_connex->response));
    } else if (user_connex && user_connex->u_name == NULL) {
        user_connex->response = "332 Need account for login.\n";
        send_all(fd, user_connex->response, strlen(user_connex->response));
    } else contains_errors(fd, user_connex, cmd);
}

static void contains_errors(int fd, connex_t *user_connex, command_t *cmd)
{
    if (strcmp("", cmd->args) != 0) {
        user_connex->response = "530 Not logged in.\n";
        send_all(fd, user_connex->response, strlen(user_connex->response));
        return;
    }
    if (user_connex && user_connex->u_name != NULL) {
        user_connex->response = "530 Not logged in.\n";
        send_all(fd, user_connex->response, strlen(user_connex->response));
    }
}