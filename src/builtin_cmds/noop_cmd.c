/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** noop_cmd
*/

#include "myftp.h"
#include <string.h>

void noop_cmd(int fd, command_t *cmd)
{
    if (strcmp("", cmd->args) == 0) {
        send_all(fd, "200 Command okay.\n", 18);
        return;
    } else
        send_all(fd, "xxx Error (RFC compliant)\n", 26);
}