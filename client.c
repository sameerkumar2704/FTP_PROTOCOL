#include<stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>


int createClient(){
    int network_socket;
    network_socket = socket(AF_INET , SOCK_STREAM , 0);

    struct sockaddr_in client_sock_address;
    client_sock_address.sin_family  = AF_INET;
    client_sock_address.sin_port = htons(9002);
    client_sock_address.sin_addr.s_addr = INADDR_ANY;

    int connection_status = connect( network_socket , (struct sockaddr*)&client_sock_address ,  sizeof(client_sock_address));
    if(connection_status == -1) {
    printf("somethig went wrong\n");
  
    }

    char message[300];
    read(network_socket , message , sizeof(message)-1);
    close(network_socket);
    printf("%s"  , message);
    return 1;
}
int main(){
	createClient();
return -1; }
