/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** dele_cmd
*/

#include "myftp.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int is_file(char *path, connex_t *user_connex);

static int can_dele(char *path, connex_t *user_connex);

static int contains_errors(int fd, connex_t *user_connex, command_t *cmd);

void dele_cmd(int fd, command_t *cmd)
{
    connex_t *user_connex = find_in_list(fd);
    char new_path[4096] = {0};

    if (contains_errors(fd, user_connex, cmd))
        return;
    resolve_path(cmd->args, user_connex, new_path, 0);
    remove(new_path);
    user_connex->response = "250 Requested file action okay, completed.\n";
    send_all(fd, user_connex->response, strlen(user_connex->response));
}

static int contains_errors(int fd, connex_t *user_connex, command_t *cmd)
{
    if (!user_connex->logged_in) {
        user_connex->response = "530 Not logged in.\n";
        send_all(fd, user_connex->response, strlen(user_connex->response));
        return (1);
    }
    if (strcmp("", cmd->args) == 0) {
        user_connex->response =
            "501 Syntax error in parameters or arguments.\n";
        send_all(fd, user_connex->response, strlen(user_connex->response));
        return (1);
    }
    if (!is_file(cmd->args, user_connex) || !can_dele(cmd->args, user_connex)) {
        user_connex->response = "550 Requested action not taken.\n";
        send_all(fd, user_connex->response, strlen(user_connex->response));
        return (1);
    }
    return (0);
}

static int is_file(char *path, connex_t *user_connex)
{
    struct stat path_stat;
    char new_path[4096] = {0};

    resolve_path(path, user_connex, new_path, 0);
    return (stat(new_path, &path_stat) == 0);
}

static int can_dele(char *path, connex_t *user_connex)
{
    struct stat path_stat;
    char new_path[4096] = {0};
    char real_path[4096] = {0};

    resolve_path(path, user_connex, new_path, 0);
    realpath(new_path, real_path);
    if (strncmp(ROOT_DIR, real_path, strlen(ROOT_DIR)) != 0) return (0);
    if (stat(real_path, &path_stat) == -1) return (0);
    if (access(real_path, R_OK | W_OK) == -1) return (0);
    return (1);
}