#include <stdio.h>
#include "../headers/create_socket.h"
#include <errno.h>
#include <signal.h>
#include "string.h"
#include <pthread.h>
#include "../headers/store_user.h"
#include "../headers/file_zip.h"
#include "../headers/reuse_func.h"
#include <dirent.h>


// function declaration 
void sendListOfUser(int socket ,ClientList *node);
void broadCastToALLUser(char *message , ClientList*parent);
void *Error_Handling(void *pointer, int LineNumber, char *FileName);
void fileTransfer(int client_socket, char *file_name);
void sendRespnceToCommand(int command, int socket, ClientList *node);
void fileReciver(int client_id, int network_socket);
void *connectReceverToClient(void *node);
void clear();
int createServer();
void signal_handler(int sig);
// ****************