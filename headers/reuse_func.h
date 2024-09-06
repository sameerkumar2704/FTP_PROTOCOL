#include <stdio.h>
int getNumberofFileInFolder(char *dir_path);
int sendFile(char *file_name, int client_socket);
long getFile_size(FILE *f);