/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** pwd_cmd
*/

#include "myftp.h"
#include <stdlib.h>
#include <string.h>

static int contains_errors(int fd, connex_t *user_connex, command_t *cmd);

static void get_wd(connex_t *user_connex);

void pwd_cmd(int fd, command_t *cmd)
{
    connex_t *user_connex = find_in_list(fd);

    if (contains_errors(fd, user_connex, cmd))
        return;
    else {
        get_wd(user_connex);
        send_all(fd, user_connex->response, strlen(user_connex->response));
        free(user_connex->response);
        send_all(fd, "\n", 1);
    }
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

static void get_wd(connex_t *user_connex)
{
    char wd[4096] = {0};

    if (strcmp(ROOT_DIR, user_connex->c_dir_path) == 0)
        user_connex->response = strdup("257 \"/\"");
    else {
        strcpy(wd, "257 \"");
        strcat(wd, user_connex->c_dir_path + strlen(ROOT_DIR));
        strcat(wd, "\"");
        user_connex->response = strdup(wd);
    }
}