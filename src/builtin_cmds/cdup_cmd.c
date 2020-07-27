/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** cdup_cmd
*/

#include "myftp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static void cdup(int fd, command_t *cmd);

static int is_dir(char *path, connex_t *user_connex);

static int is_subdir(char *path, connex_t *user_connex);

static int contains_errors(int fd, connex_t *user_connex, command_t *cmd,
    int check_num);

void cdup_cmd(int fd, command_t *cmd)
{
    connex_t *user_connex = find_in_list(fd);

    if (strlen(cmd->args) == 0 &&
        user_connex->c_dir_path[strlen(user_connex->c_dir_path) - 1] != '/') {
        cmd->args = calloc(strlen(user_connex->c_dir_path) + 4, sizeof(char));
        strcat(cmd->args, user_connex->c_dir_path);
        strcat(cmd->args, "/..");
        cdup(fd, cmd);
    } else if (strlen(cmd->args) == 0) {
        cmd->args = calloc(strlen(user_connex->c_dir_path) + 3, sizeof(char));
        strcat(cmd->args, user_connex->c_dir_path);
        strcat(cmd->args, "..");
        cdup(fd, cmd);
    } else contains_errors(fd, user_connex, cmd, 1);
}

static void cdup(int fd, command_t *cmd)
{
    connex_t *user_connex = find_in_list(fd);

    if (contains_errors(fd, user_connex, cmd, 2))
        return;
    else {
        closedir(user_connex->c_dir);
        user_connex->c_dir = opendir(user_connex->c_dir_path);
        user_connex->response = "200 Command okay.\n";
        send_all(fd, user_connex->response, strlen(user_connex->response));
    }
}

static int is_dir(char *path, connex_t *user_connex)
{
    struct stat path_stat;
    char new_path[4096] = {0};

    resolve_path(path, user_connex, new_path, 1);
    printf("Root dir: %s\n", ROOT_DIR);
    printf("Dir name: %s\n\n", new_path);
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

    resolve_path(path, user_connex, new_path, 1);
    realpath(new_path, real_path);
    printf("Root dir: %s\n", ROOT_DIR);
    printf("Dir name: %s\n\n", new_path);
    if (strncmp(ROOT_DIR, real_path, strlen(ROOT_DIR)) == 0) {
        free(user_connex->c_dir_path);
        user_connex->c_dir_path = strdup(real_path);
        return (1);
    }
    return (0);
}

static int contains_errors(int fd, connex_t *user_connex, command_t *cmd,
    int check_num)
{
    if (!user_connex->logged_in) {
        user_connex->response = "530 Not logged in.\n";
        send_all(fd, user_connex->response, strlen(user_connex->response));
        return (1);
    }
    if (strcmp("", cmd->args) != 0 && check_num == 1) {
        user_connex->response =
            "501 Syntax error in parameters or arguments.\n";
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