/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** resolve_path
*/

#include "myftp.h"
#include <string.h>

void resolve_path(char *path, connex_t *user_connex, char *new_path,
    int cdup)
{
    if (path[0] == '.' || (path[0] != '/' && path[0] != '~')) {
        strcat(new_path, user_connex->c_dir_path);
        if (new_path[strlen(new_path) - 1] != '/') {
            new_path[strlen(new_path)] = '/';
            new_path[strlen(new_path)] = '\0';
        }
        strcat(new_path, path);
    } else if (path[0] == '/' || path[0] == '~') {
        if (!cdup) strcat(new_path, ROOT_DIR);
        if (new_path[strlen(new_path) - 1] != '/') {
            new_path[strlen(new_path)] = '/';
            new_path[strlen(new_path)] = '\0';
        }
        strcat(new_path, path + 1);
    }
}