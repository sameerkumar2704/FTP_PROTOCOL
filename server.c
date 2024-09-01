#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "./headers/create_socket.h"
#include <errno.h>
#include <signal.h>
#include <zip.h>
#include "string.h"
#include <pthread.h>
#include "./headers/store_user.h"

extern int errno;
#define PORT_NO 9002
#define LINE_NUMBER __LINE__
#define FILE_NAME __FILE__

int server_network = -1; // server id

ClientList *header = NULL;
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
      int status = sendMessage(client_socket, server_message);
      if (status == -1)
      {
         perror("Data not send :");
         return;
      }
      printf("%s\n", server_message);
   }
   fclose(f);
}
void *connectReceverToClient(void *node)
{
   ClientList *curr_node = (ClientList *)node;
   int client_id = curr_node->client_id;
   char message[256];
   int n = -1;
   if (header != curr_node)
   {
      printf("%s\n", message);
      send(header->client_id, "hellow from sameer", 256, 0);
   }
   // while ((n = recv(client_id, message, sizeof(message), 0)) > 0)
   // {
   // }
   if (n == 0)
   {
      perror("client disconnect");
   }
}

void zip_files(char *path)
{
   int err = 0;
   zip_t *zip = zip_open(path, ZIP_CREATE, &err);
   zip_source_t *source = zip_source_file(zip, "send_file.txt", 0, 0);
   zip_file_add(zip, "send_file.txt", source, ZIP_FL_OVERWRITE);
   zip_close(zip);
}
int createServer()

{
   char *ip = "127.0.0.1";
   server_network = createTcpSocket();

   int opt = 1;

   setsockopt(server_network, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

   struct sockaddr_in *socket_address = createTcpIpV4SocketAddress("", PORT_NO);

   int binded = bind(server_network, (struct sockaddr *)socket_address, sizeof(*socket_address));
   if (binded == -1)
   {
      printf("Address is aleary in user\n");
      return -1;
   }
   zip_files("send_file.zip");
   printf("server live at port  : %d\n", PORT_NO);
   int isListening = listen(server_network, 10);
   if (isListening == -1)
   {
      perror("server not able listen : ");
      return -1;
   }

   while (1)
   {
      struct sockaddr_in tem_address;
      int client_socket = accept(server_network, NULL, NULL);
      ClientList *node = createClientNode(client_socket, &tem_address);
      if (header == NULL)
      {
         header = node;
      }
      else
      {
         addNewClient(header, node);
      }
      char message[1024] = "you are connected";
      if ((sendMessage(client_socket, message) < 0))
      {
         perror("sorry :");
      }
      pthread_t t1;
      pthread_create(&t1, NULL, &connectReceverToClient, node);

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
