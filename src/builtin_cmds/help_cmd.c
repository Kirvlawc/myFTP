/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** help_cmd
*/

#include "myftp.h"
#include <string.h>

void help_cmd(int fd, command_t *cmd)
{
    if (strcmp("", cmd->args) == 0) {
        send_all(fd, "214 System OK. Enter a specfic for more information"
            " on it.\n", 59);
        return;
    } else {
        for (int i = 0; i < NUM_CMDS; i++) {
            if (strcmp(cmd->args, CMDS_STR[i]) == 0) {
                send_all(fd, HELP_MSGS[i], strlen(HELP_MSGS[i]));
                return;
            }
        }
    }
    send_all(fd, "xxx Error (RFC compliant)\n", 26);
}