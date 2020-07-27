/*
** EPITECH PROJECT, 2020
** NWP_myftp_2019
** File description:
** Main function for my_ftp project
*/

#include "myftp.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char *ROOT_DIR;

char **ENV_LIST;

int is_dir(char *path);

static int is_number(char *num);

int main(int argc, char *argv[], char **envp)
{
    if (argc == 2 && strcmp(argv[1], "-help") == 0) {
        printf("USAGE:\t./myftp port path\n"
            "\tport is the port number on which the server socket listens\n"
            "\tpath is the path to the home directory for the Anonymous user\n"
        );
    } else if (argc == 3 && is_number(argv[1]) && is_dir(argv[2])) {
        ENV_LIST = envp;
        launch_server(argv[1]);
    } else {
        printf("USAGE:\t./myftp port path\n"
            "\tport is the port number on which the server socket listens\n"
            "\tpath is the path to the home directory for the Anonymous user\n"
            );
        return (84);
    }
}

int is_dir(char *path)
{
    struct stat path_stat;

    if (stat(path, &path_stat) == -1) return (0);
    if (S_ISDIR(path_stat.st_mode)) {
        if (access(path, R_OK | W_OK) == -1) {
            perror("access");
            exit(84);
        }
        ROOT_DIR = realpath(path, NULL);
        return (1);
    }
    return (0);
}

static int is_number(char *num)
{
    for (int i = 0; num[i] != '\0'; i++) {
        if (!isdigit(num[i]))
            return (0);
    }
    return (1);
}