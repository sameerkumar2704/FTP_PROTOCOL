#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include "./headers/create_socket.h"
#include <arpa/inet.h>
#include <errno.h>
extern int errno;
#define PORT_NO 9002
#define LINE_NUMBER __LINE__
#define FILE_NAME __FILE__
#include "string.h"
#include <zip.h>

void *Error_Handling(void *pointer, int LineNumber, char *FileName)
{
   if (pointer == NULL)
   {
      printf("Error in %s at Line : %d \n%s", FileName, LineNumber, strerror(errno));
      return NULL;
   }
   return pointer;
}
int createServer()
{
   char *ip = "127.0.0.1";
   int server_network = createTcpSocket();
   char server_message[1024];
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

   int client_socket = accept(server_network, NULL, NULL);
   FILE *f = fopen("send_file.zip", "rb");
   int size;
   while ((size = fread(server_message, sizeof(char), sizeof(server_message), f)) > 0)
   {
      send(client_socket, server_message, size, 0);
      printf("%s\n", server_message);
   }
   fclose(f);
   return -1;
}
void zipFile()
{
   FILE *file = fopen("./client_data/file.zip", "wb");
   FILE *f = fopen("./testing_data/send_file.zip", "rb");
   char server_message[1024];

   int size;
   while ((size = fread(server_message, sizeof(char), sizeof(server_message), f)) > 0)
   {
      fwrite(server_message, sizeof(char), size, file);
      printf("%s\n", server_message);
   }
}
int main()
{
   createServer();
   return -1;
}
