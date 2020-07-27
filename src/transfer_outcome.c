/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** send_file_outcome
*/

#include "myftp.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void read_failure(connex_t *user_connex)
{
    user_connex->response = "451 Requested action aborted."
        " Local error in processing.\n";
    send_all(user_connex->sock_fd, user_connex->response,
        strlen(user_connex->response));
    exit(84);
}

void write_failure(connex_t *user_connex)
{
    user_connex->response = "451 Requested action aborted."
        " Local error in processing.\n";
    send_all(user_connex->sock_fd, user_connex->response,
        strlen(user_connex->response));
    exit(84);
}

void connex_open_failure(connex_t *user_connex)
{
    user_connex->response = "425 Can't open data connection.\n";
    send_all(user_connex->sock_fd, user_connex->response,
        strlen(user_connex->response));
    exit(84);
}

void connex_close_error(connex_t *user_connex)
{
    user_connex->response = "426 Connection closed; transfer aborted.\n";
    send_all(user_connex->sock_fd, user_connex->response,
        strlen(user_connex->response));
    exit(84);
}