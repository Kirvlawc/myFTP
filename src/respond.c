/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** Respond to the entered command
*/

#include "myftp.h"
#include <stdio.h>
#include <string.h>

void respond(int fd, command_t *cmd)
{
    for (int i = 0; i < NUM_CMDS; i++) {
        if (strcmp(cmd->cmd, CMDS_STR[i]) == 0 && cmd_funcs[i]) {
            cmd_funcs[i](fd, cmd);
            return;
        }
    }
    send_all(fd, "500 Syntax error, command unrecognized.\n", 40);
}