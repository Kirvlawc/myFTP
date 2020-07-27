/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** send_file
*/

#include "myftp.h"
#include <unistd.h>

static int send_part(int client_fd, const char *buffer, int len);

void send_file(int src_fd, int dest_fd, size_t size, connex_t *user_connex)
{
    char buff[4096];
    ssize_t num_read = 0;
    ssize_t num_sent = 0;
    size_t total = 0;

    while (size > 0) {
        num_read = read(src_fd, buff, 4096);
        if (num_read == -1)
            read_failure(user_connex);
        if (num_read == 0)
            break;
        num_sent = send_part(dest_fd, buff, num_read);
        if (num_sent == -1)
            write_failure(user_connex);
        if (num_sent == 0)
            write_failure(user_connex);
        size -= num_sent;
        total += num_sent;
    }
}

static int send_part(int client_fd, const char *buffer, int len)
{
    int rem = len;
    int sent;
    int total = 0;

    while (rem != 0) {
        sent = write(client_fd, buffer + total, rem);
        if (sent == -1)
            return (-1);
        total += sent;
        rem -= sent;
    }
    return (total);
}