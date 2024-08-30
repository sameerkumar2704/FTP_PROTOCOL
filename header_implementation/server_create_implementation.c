#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>


int createTcpSocket(){
    return socket( AF_INET , SOCK_STREAM , 0);
}
struct sockaddr_in * createTcpIpV4SocketAddress(char * ip , int port_number){
   struct sockaddr_in *socket_address = malloc(sizeof(struct sockaddr_in));
   socket_address->sin_family  = AF_INET;
   socket_address->sin_port = htons(port_number);
   socket_address->sin_addr.s_addr = INADDR_ANY;

   return socket_address;


}
