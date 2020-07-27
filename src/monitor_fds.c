/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** monitor_fds
*/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void monitor_fds(int fd_max, fd_set *read_fds, fd_set *master)
{
    struct stat stat_buf;

    for (int i = 0; i <= fd_max; i++) {
        if (fstat(i, &stat_buf) == -1) {
            FD_CLR(i, read_fds);
            FD_CLR(i, master);
            printf("Deleted\n");
        }
    }
    select(fd_max + 1, read_fds, NULL, NULL, NULL);
}