#include<stdio.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include "./headers/create_socket.h"
#include "errno.h"
extern int errno;
#define PORT_NO 9002
#define LINE_NUMBER __LINE__
#define FILE_NAME __FILE__
#include "string.h"

void* Error_Handling (void * pointer , int LineNumber ,char* FileName){
   if(pointer == NULL){
      printf("Error in %s at Line : %d \n%s" , FileName , LineNumber , strerror(errno) );
      return NULL;
   }
   return pointer;
}
int createServer(){
   int server_network = createTcpSocket();
   char server_message[300] = "connected  \n";

   struct sockaddr_in* socket_address = createTcpIpV4SocketAddress("", PORT_NO);


   bind(server_network , (struct sockaddr*) socket_address , sizeof(*socket_address));

   FILE * f = Error_Handling(fopen("./testing_data/send_file.txt" , "r") , LINE_NUMBER , FILE_NAME);
   if(f == NULL){
      printf("Server Stoped");
      return -1;
   }
   printf("listening to port : 9002\n");

   listen( server_network , 10);

   int client_socket = accept( server_network , NULL , NULL );

   while(!feof(f)){
      fread(server_message , sizeof(char) , sizeof(server_message) , f);
      send(client_socket , server_message , sizeof(server_message) , 0 );
   }
   fclose(f);
   close(server_network);
   return -1;

}

int main() {
   createServer();
   return -1;
}
