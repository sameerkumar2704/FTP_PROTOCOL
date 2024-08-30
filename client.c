#include<stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "./headers/create_socket.h"

#define PORT_NO  9002
int createClient(){
    int network_socket = createTcpSocket();

    struct sockaddr_in * client_sock_address =  createTcpIpV4SocketAddress("", PORT_NO);


    int connection_status = connect( network_socket , (struct sockaddr*)client_sock_address ,  sizeof(*client_sock_address));
    FILE *file = fopen("./testing_data/recieve.txt" ,"w");
    if(connection_status == -1) {
    printf("somethig went wrong\n");
    fclose(file);
    close(network_socket);
    return -1;

    }
    char message[300];
    int file_count = recv(network_socket , message , sizeof(message)-1 , 0);
    fputs( message , file);

    fclose(file);
    close(network_socket);
    return 1;
}
int main(){
	createClient();
return -1; }
