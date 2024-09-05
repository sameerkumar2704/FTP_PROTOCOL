#include <stdio.h>
#include "./headers/create_socket.h"
#include <errno.h>
#include <signal.h>
#include "string.h"
#include <pthread.h>
#include "./headers/store_user.h"
#include "./headers/file_zip.h"

extern int errno;
#define PORT_NO 9002
#define LINE_NUMBER __LINE__
#define FILE_NAME __FILE__
enum
{
   SHOW_ALL_USERS,
   READ_TO_RECIVE_DATA,
   SERVER_SHUT_DOWN,
   SELECT_USER,
   ABORT_DATA_SENDING,
};
int server_network = -1; // server id

ClientList *header = NULL;
void sendListOfUser(int socket, ClientList *node)
{
   ClientList *curr = header;
   int count = 0;
   while (curr != NULL)
   {
      if (curr != node)
      {
         char buffer[1024];
         snprintf(buffer, sizeof(buffer), "user id - %d", curr->client_id);
         sendMessage(socket, buffer);
      }
      count++;
      curr = curr->next_user;
   }
   if (count == 1)
      sendMessage(socket, "no user other than you");
}

void broadCastToALLUser(char *message, ClientList *parent)
{
   ClientList *curr = header;
   while (curr != NULL)
   {
      if (curr != parent)
      {
         sendMessage(curr->client_id, message);
      }
      curr = curr->next_user;
   }
}
void *Error_Handling(void *pointer, int LineNumber, char *FileName)
{
   if (pointer == NULL)
   {
      printf("Error in %s at Line : %d \n%s", FileName, LineNumber, strerror(errno));
      return NULL;
   }
   return pointer;
}

void sendFile(int client_socket)
{
   char server_message[1024];
   FILE *f = fopen("temp.zip", "rb");
   fseek(f, 0, SEEK_END);
   long file_size = ftell(f);
   fseek(f, 0, SEEK_SET);
   send(client_socket, &file_size, sizeof(file_size), 0);
   int size;
   while (1)
   {
      int size = fread(server_message, sizeof(char), sizeof(server_message), f);
      if (size == 0)
         break;
      int status = send(client_socket, server_message, size, 0);
      if (status == -1)
      {
         perror("Data not send :");
         return;
      }
      printf("%s\n", server_message);
   }
   printf("Data is send to client \n");
   fclose(f);
   remove("temp.zip");
}

void sendRespnceToCommand(int command, int socket, ClientList *node)
{
   switch (command)
   {
   case SHOW_ALL_USERS:
      sendListOfUser(socket, node);
      break;
   case SELECT_USER:
      sendMessage(socket, "provide-user-id");
      break;
   default:
      sendMessage(socket, "404 command not found");
   }
}
void fileReciver(int client_id, int network_socket)
{
   long  size_of_file ;
   char buffer[1024];

   recv(network_socket, &size_of_file, sizeof(size_of_file), 0);
   printf("file server %ld\n", size_of_file);
   FILE *file = fopen("temp.zip", "wb");
   long curr_size = 0;
   char message[1024] = "yes";
   sendMessage(client_id, message);
  
   char* data[size_of_file/1024];
   int i = 0;
   while (curr_size < size_of_file)
   {
      size_t byte_size = recv(network_socket, buffer, sizeof(buffer), 0);
      fwrite(buffer, sizeof(char), byte_size, file);
      curr_size += byte_size;
   }
   fclose(file);
 
   sendFile(client_id);
   printf("file completly send to user %d", client_id);
}
void *connectReceverToClient(void *node)
{
   ClientList *curr_node = (ClientList *)node;
   int client_id = curr_node->client_id;
   int command = -1;
   int n = -1;
   int selected_user_to_send_data;
   while ((n = recv(client_id, &command, sizeof(command), 0)) > 0)
   {
      sendRespnceToCommand(command, client_id, curr_node);
      if (command == SELECT_USER)
      {
         recv(client_id, &selected_user_to_send_data, sizeof(selected_user_to_send_data), 0);
         fileReciver(selected_user_to_send_data, client_id);
         printf("end\n");
      }
   }
   if (n == 0)
   {
      ClientList *element = findClient(header, curr_node);
      if (element != NULL)
         printf("found element\n");
      header = removeClient(header, curr_node);
      element = findClient(header, curr_node);

      if (element == NULL)
         printf("node removed\n");

      ClientList *curr = header;
      while (curr != NULL)
      {

         char disconnect_messag[1024] = "user id - ";
         snprintf(disconnect_messag, 32, "connecte id - %d", curr->client_id);
         broadCastToALLUser(disconnect_messag, curr);

         curr = curr->next_user;
      }

      perror("client disconnect ");
   }
}
void clear()
{
   while (header != NULL)
   {
      sendMessage(header->client_id, "server-down");
      header = header->next_user;
   }
   printf("\nStopping server...\n");
   shutdown(server_network, SHUT_RDWR);
   close(server_network);
   exit(0);
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
      pthread_t t1;
      pthread_create(&t1, NULL, &connectReceverToClient, node);
   }
   clear();

   return -1;
}

void signal_handler(int sig)
{
   clear();
}
int main()
{
   signal(SIGINT, signal_handler);
   signal(SIGTERM, signal_handler);
   createServer();
   return -1;
}
