/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** send_list
*/

#include "myftp.h"
#include <stdio.h>
#include <unistd.h>

void send_list(int dest_fd, char *path)
{
    char *args[4] = {"/usr/bin/ls", "-l", path, NULL};

    close(fileno(stdout));
    dup(dest_fd);
    execve(args[0], args, ENV_LIST);
}