#include <dirent.h>
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "../headers/create_socket.h"
int getNumberofFileInFolder(char *dir_path)
{
    DIR *dir;
    struct dirent *entry;
    int file_count = 0;

    dir = opendir(dir_path);
    if (dir == NULL)
    {
        return -1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        // Ignore directories '.' and '..'
        if (entry->d_type == DT_REG)
        {
            // counting files
            file_count++;
        }
    }
    return file_count;
}

long getFile_size(FILE *f)
{
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    return file_size;
}

int sendFile(char *file_name, int client_socket)
{
    char server_message[1024];
    FILE *f = fopen(file_name, "rb");
    long file_size = getFile_size(f);
    send(client_socket, &file_size, sizeof(file_size), 0);
    int size;
    while (1)
    {
        int size = fread(server_message, sizeof(char), sizeof(server_message), f);
        if (size == 0)
            break;
        int status = send(client_socket, server_message, size, 0);
        if (status == -1)
        {
            return -1;
        }
    }
    fclose(f);
    return 1;
}
