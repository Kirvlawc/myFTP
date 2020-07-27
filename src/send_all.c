/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** send_all
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void send_all(int client_fd, const char *buffer, int len)
{
    int rem = len;
    int sent;
    int total = 0;

    while (rem != 0) {
        sent = write(client_fd, buffer + total, rem);
        if (sent == -1) {
            perror("write");
            exit(84);
        }
        total += sent;
        rem -= sent;
    }
}