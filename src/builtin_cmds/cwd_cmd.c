/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** cwd_cmd
*/

#include "myftp.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int contains_errors(int fd, connex_t *user_connex, command_t *cmd);

static int is_dir(char *path, connex_t *user_connex);

static int is_subdir(char *path, connex_t *user_connex);

void cwd_cmd(int fd, command_t *cmd)
{
    connex_t *user_connex = find_in_list(fd);

    if (contains_errors(fd, user_connex, cmd))
        return;
    else {
        closedir(user_connex->c_dir);
        user_connex->c_dir = opendir(user_connex->c_dir_path);
        user_connex->response = "250 Requested file action okay, completed.\n";
        send_all(fd, user_connex->response, strlen(user_connex->response));
    }
}

static int is_dir(char *path, connex_t *user_connex)
{
    struct stat path_stat;
    char new_path[4096] = {0};

    resolve_path(path, user_connex, new_path, 0);
    if (stat(new_path, &path_stat) == -1) return (0);
    if (S_ISDIR(path_stat.st_mode)) {
        if (access(new_path, R_OK | W_OK) == 0)
            return (1);
    }
    return (0);
}

static int is_subdir(char *path, connex_t *user_connex)
{
    char new_path[4096] = {0};
    char real_path[4096] = {0};

    resolve_path(path, user_connex, new_path, 0);
    realpath(new_path, real_path);
    if (strncmp(ROOT_DIR, real_path, strlen(ROOT_DIR)) == 0) {
        free(user_connex->c_dir_path);
        user_connex->c_dir_path = strdup(real_path);
        return (1);
    }
    return (0);
}

static int contains_errors(int fd, connex_t *user_connex, command_t *cmd)
{
    if (!user_connex->logged_in) {
        user_connex->response = "530 Not logged in.\n";
        send_all(fd, user_connex->response, strlen(user_connex->response));
        return (1);
    }
    if (strcmp("", cmd->args) == 0) {
        user_connex->response = "550 Requested action not taken.\n";
        send_all(fd, user_connex->response, strlen(user_connex->response));
        return (1);
    }
    if (!is_dir(cmd->args, user_connex) || !is_subdir(cmd->args, user_connex)) {
        user_connex->response = "550 Requested action not taken.\n";
        send_all(fd, user_connex->response, strlen(user_connex->response));
        return (1);
    }
    return (0);
}