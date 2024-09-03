#include <stdio.h>
#include "./headers/create_socket.h"
#include "string.h"
#include <pthread.h>

#define PORT_NO 9002
#define buffer_size 1024
int network_socket = -1;
 enum  {SHOW_ALL_USERS , READ_TO_RECIVE_DATA , SERVER_SHUT_DOWN };
int findingCommandType(char * command){
    if(!strcmp(command , "show-all-users\n")) return SHOW_ALL_USERS;
    if(!strcmp(command , "file-reciver-on\n")) return READ_TO_RECIVE_DATA;
    return -1;
}
void sendCommandToServer(int commandType){
    
    switch (commandType)
    {
    case SHOW_ALL_USERS:
        send(network_socket , &commandType , sizeof(commandType) , 0 );
        break;
    case READ_TO_RECIVE_DATA:
        send(network_socket , &commandType , sizeof(commandType) , 0 );
        break;
    
    default:
        printf("404 command not found \n");
        break;
    }
}
void fileReciver(){
    long size_of_file = 0;
    char buffer[buffer_size];
    FILE *file = fopen("./client_data/file.zip", "wb");
    recv(network_socket , &size_of_file , sizeof(size_of_file) , 0);
    printf("file size : %ld\n" , size_of_file);
    long curr_size = 0;
    while(curr_size<size_of_file){
        size_t byte_size = recv(network_socket , buffer  , sizeof(buffer)  ,0  );
        fwrite(buffer, sizeof(char), byte_size, file);
        curr_size += byte_size;

    }
    fclose(file);
}
void closeSocket()
{
    shutdown(network_socket, SHUT_RDWR);
    close(network_socket);
}
void *serverResponseHandler()
{
    char buffer[buffer_size];
    int n = 0;
    while(1){
        n = recvMessage(network_socket , buffer , buffer_size);
        if(!strcmp("server-down" , buffer) || n == 0 ) {
            printf("%s\n" , buffer);
            closeSocket();
            break;
        }
        printf("server res : %s\n" , buffer);
        if(!strcmp(buffer , "yes")){
            fileReciver();
        }
    }
    

    
}
int createClient()
{
    network_socket = createTcpSocket();
    struct sockaddr_in *socket_address = createTcpIpV4SocketAddress("", PORT_NO);
    int connection_status = connect(network_socket, (struct sockaddr *)socket_address, sizeof(*socket_address));

    if (connection_status == -1)
    {
        printf("somethig went wrong\n");
        close(network_socket);
        return -1;
    }

   

    return 1;
}

int main()
{
   
    if( createClient() == -1){
        printf("server is not connect \n");
        return -1;
    }
    pthread_t t1;
    pthread_create(&t1, NULL, &serverResponseHandler, NULL);
    char command[buffer_size];
    while(1){
        fgets(command , buffer_size , stdin);
        if(!strcmp(command , "exit\n")) {
            break;
        }else{
            sendCommandToServer(findingCommandType(command));
        }
        memset(command , 0 , sizeof(command));
    }
    closeSocket();
    return -1;
}
