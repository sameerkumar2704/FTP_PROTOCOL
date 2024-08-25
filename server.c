#include<stdio.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>

int createServer(){
   int server_network;
   char server_message[300] = "connected  \n";
   server_network = socket( AF_INET , SOCK_STREAM , 0);
   struct sockaddr_in socket_address;
   socket_address.sin_family  = AF_INET;
   socket_address.sin_port = htons(9002);
   socket_address.sin_addr.s_addr = INADDR_ANY;

   bind(server_network , (struct sockaddr*) &socket_address , sizeof(socket_address));
   printf("listning to port 9002\n");
   listen( server_network , 3);

   int client_socket;
   client_socket = accept( server_network , NULL , NULL );
   send(client_socket , server_message , sizeof(server_message) , 0 );
   close(server_network);

}

int main() {
   createServer();
   return -1;
}
