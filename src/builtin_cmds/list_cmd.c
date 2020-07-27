/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** list_cmd
*/

#include "myftp.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

static int contains_errors(int fd, connex_t *user_connex, command_t *cmd);

static void list_files(char *path, connex_t *user_connex, int client_fd);

void list_cmd(int fd, command_t *cmd)
{
    char new_path[4096] = {0};

    connex_t *user_connex = find_in_list(fd);
    if (contains_errors(fd, user_connex, cmd))
        return;
    resolve_path(cmd->args, user_connex, new_path, 0);
    list_files(new_path, user_connex, fd);
}

static void list_files(char *path, connex_t *user_connex, int client_fd)
{
    pid_t pid = fork();

    if (pid == 0) {
        if (user_connex->type == PASSIVE)
            listfiles_pasv(path, user_connex, client_fd);
        else if (user_connex->type == ACTIVE)
            listfiles_act(path, user_connex, client_fd);
        exit(0);
    } else {
        if (user_connex->type == PASSIVE)
            user_connex->pasv_fd = -1;
        else if (user_connex->type == ACTIVE) {
            free(user_connex->act_ip);
            free(user_connex->act_port);
            user_connex->act_ip = NULL;
            user_connex->act_port = NULL;
        }
        user_connex->type = NONE;
    }
}

static int contains_errors(int fd, connex_t *user_connex, command_t *cmd)
{
    if (!user_connex->logged_in) {
        user_connex->response = "530 Not logged in.\n";
        send_all(fd, user_connex->response, strlen(user_connex->response));
        return (1);
    }
    if (user_connex->type == NONE) {
        user_connex->response = "425 Use PORT or PASV first.\n";
        send_all(fd, user_connex->response, strlen(user_connex->response));
        return (1);
    }
    return (0);
}