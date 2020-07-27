/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** Connection list manipulations
*/

#include "myftp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void alloc_connex(connex_t **list, int fd);

static int found_space(int fd);

void add_to_list(int fd)
{
    connex_t *list_cpy = connect_list;
    connex_t *new_connex = NULL;

    if (connect_list == NULL)
        alloc_connex(&connect_list, fd);
    else {
        if (found_space(fd)) return;
        while (list_cpy->next != NULL)
            list_cpy = list_cpy->next;
        alloc_connex(&new_connex, fd);
        list_cpy->next = new_connex;
    }
}

static void alloc_connex(connex_t **list, int fd)
{
    if ((*list = malloc(sizeof(connex_t))) == NULL) {
        perror("malloc");
        exit(84);
    }
    (*list)->sock_fd = fd;
    (*list)->pasv_fd = -1;
    (*list)->u_name = NULL;
    (*list)->logged_in = 0;
    (*list)->response = "";
    (*list)->c_dir_path = strdup(ROOT_DIR);
    (*list)->c_dir = opendir((*list)->c_dir_path);
    (*list)->act_ip = NULL;
    (*list)->act_port = NULL;
    (*list)->type = NONE;
    (*list)->next = NULL;
}

static int found_space(int fd)
{
    connex_t *list_cpy = connect_list;

    while (list_cpy != NULL) {
        if (list_cpy->sock_fd == -1) {
            list_cpy->sock_fd = fd;
            list_cpy->pasv_fd = -1;
            list_cpy->u_name = NULL;
            list_cpy->logged_in = 0;
            list_cpy->response = "";
            list_cpy->c_dir_path = strdup(ROOT_DIR);
            list_cpy->c_dir = opendir(list_cpy->c_dir_path);
            list_cpy->act_ip = NULL;
            list_cpy->act_port = NULL;
            list_cpy->type = NONE;
            return (1);
        }
        list_cpy = list_cpy->next;
    }
    return (0);
}

void delete_from_list(int fd)
{
    connex_t *list_cpy = connect_list;

    while (list_cpy != NULL) {
        if (list_cpy->sock_fd == fd) {
            list_cpy->sock_fd = -1;
            list_cpy->pasv_fd = -1;
            list_cpy->u_name = NULL;
            list_cpy->logged_in = 0;
            list_cpy->response = "";
            free(list_cpy->c_dir_path);
            list_cpy->c_dir_path = NULL;
            closedir(list_cpy->c_dir);
            list_cpy->c_dir = NULL;
            list_cpy->act_ip = NULL;
            list_cpy->act_port = NULL;
            list_cpy->type = NONE;
            return;
        }
        list_cpy = list_cpy->next;
    }
}

connex_t *find_in_list(int fd)
{
    connex_t *list_cpy = connect_list;

    while (list_cpy != NULL) {
        if (list_cpy->sock_fd == fd) {
            return (list_cpy);
        }
        list_cpy = list_cpy->next;
    }
    return (NULL);
}