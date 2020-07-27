/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** recv_file
*/

#include <myftp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void recv_file(int src_fd, int dest_fd, connex_t *user_connex)
{
    static char buff[4096];
    int length = 0;

    while ((length = read(src_fd, buff, 4096)) > 0)
        send_all(dest_fd, buff, length);
    if (length == -1)
        read_failure(user_connex);
}