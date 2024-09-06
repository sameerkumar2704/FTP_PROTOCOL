#include <stdio.h>
#include "../headers/create_socket.h"
#include "string.h"
#include <pthread.h>
#include "../headers/file_zip.h"
#include "../headers/reuse_func.h"
#include <zip.h>
#include <sys/stat.h>
#include "../headers/store_user.h"
// function declaration

int findingCommandType(char *command);
int get_UserId(char *command , int reduce);
void zip_file_to_send_server(char *sourc_file , char*file_name);
void get_detail_of_file_and_path_name(int commandType);
void sendCommandToServer(int commandType);
void fileReciver();
void closeSocket();
void *serverResponseHandler();
int createClient();

// ***********