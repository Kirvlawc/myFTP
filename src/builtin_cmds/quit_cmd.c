/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** quit_cmd
*/

#include "myftp.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void quit_cmd(int fd, command_t *cmd)
{
    connex_t *user_connex = find_in_list(fd);

    user_connex->response = "221 Service closing control connection.\n";
    send_all(fd, user_connex->response, strlen(user_connex->response));
    if (user_connex->act_ip)
        free(user_connex->act_ip);
    if (user_connex->act_port)
        free(user_connex->act_port);
    delete_from_list(fd);
    close(fd);
}