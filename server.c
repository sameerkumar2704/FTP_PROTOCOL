#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include "./headers/create_socket.h"
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <zip.h>
#include "string.h"
#include <pthread.h>

extern int errno;
#define PORT_NO 9002
#define LINE_NUMBER __LINE__
#define FILE_NAME __FILE__

typedef struct
{
   int socket_id;
   struct sockaddr_in *address;
   /* data */
} Clinet;
Clinet clients[10];
int count = 0;
int server_network = -1; // server id
void *Error_Handling(void *pointer, int LineNumber, char *FileName)
{
   if (pointer == NULL)
   {
      printf("Error in %s at Line : %d \n%s", FileName, LineNumber, strerror(errno));
      return NULL;
   }
   return pointer;
}
void performClientTask(int client_socket)
{
   char server_message[1024];
   FILE *f = fopen("send_file.zip", "rb");
   int size;
   while ((size = fread(server_message, sizeof(char), sizeof(server_message), f)) > 0)
   {
      send(client_socket, server_message, size, 0);
      printf("%s\n", server_message);
   }
   char stop_message[10] = "shutdown";
   ;
   if ((send(client_socket, stop_message, strlen(stop_message), 0)) < 0)
   {
      printf("%s\n", "something_wend_wrong");
      exit(1);
   }
   fclose(f);
}
void *connectReceverToClient()
{
   printf("%d\n", count);
   int client_id = clients[count].socket_id;
   count++;
   char message[256];
   int n = -1;
   while ((n = recv(client_id, message, sizeof(message), 0)) > 0)
   {

      // if (count - 1 > 0)
      // {
      //    printf("%s\n", message);
      //    send(clients[0].socket_id, "hellow from sameer", 256, 0);
      // }
   }
   if (n == 0)
   {
      perror("client disconnect");
   }
}

int createServer()

{
   char *ip = "127.0.0.1";
   server_network = createTcpSocket();

   int opt = 1;
   int err = 0;
   setsockopt(server_network, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

   zip_t *zip = zip_open("send_file.zip", ZIP_CREATE, &err);
   zip_source_t *source = zip_source_file(zip, "send_file.txt", 0, 0);
   zip_file_add(zip, "send_file.txt", source, ZIP_FL_OVERWRITE);
   zip_close(zip);

   struct sockaddr_in *socket_address = createTcpIpV4SocketAddress("", PORT_NO);

   int binded = bind(server_network, (struct sockaddr *)socket_address, sizeof(*socket_address));
   if (binded == -1)
   {
      printf("Address is aleary in user\n");
      return -1;
   }

   char header[] = "name: send_file.txt";
   printf("listening to port : 9002\n");
   listen(server_network, 10);

   while (1)
   {
      int client_socket = accept(server_network, (struct sockaddr *)clients[count].address, NULL);
      clients[count].socket_id = client_socket;
      send(clients[count].socket_id, "your are connected", 256, 0);
      pthread_t t1;
      pthread_create(&t1, NULL, &connectReceverToClient, NULL);

      // performClientTask(clients[0].socket_id);
   }

   return -1;
}

void signal_handler(int sig)
{
   printf("\nStopping server...\n");
   close(server_network);
   exit(0);
}
int main()
{
   signal(SIGINT, signal_handler);
   createServer();
   return -1;
}
