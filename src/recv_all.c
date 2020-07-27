/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** recv_all
*/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void check_end_line(char *buffer, int buff_size);

char *recv_all(int client_fd)
{
    static char buff[256];
    char *input = NULL;
    int length = 0;
    int total = 0;

    while ((length = read(client_fd, buff, 255)) > 0) {
        check_end_line(buff, length);
        length = (length == (int)strlen(buff)) ? length : (int)strlen(buff);
        total += length;
        if (total == length)
            input = calloc(total + 1, sizeof(char));
        else
            input = realloc(input, total);
        strcat(input, buff);
        input[total] = '\0';
        if (length != 255) {
            memmove(buff, buff + length, 255 - length);
            break;
        } else memset(buff, 0, 256);
    }
    return (input);
}

void check_end_line(char *buffer, int buff_size)
{
    for (int i = 0; i < buff_size; i++) {
        if (buffer[i] == '\0' || buffer[i] == '\r' || buffer[i] == '\n') {
            buffer[i] = '\0';
            return;
        }
    }
}