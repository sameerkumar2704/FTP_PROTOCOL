#include "./server_interface.h"
// constnat data accross file

extern int errno;
#define PORT_NO 9002
#define buffer_size 1024
ClientList *header = NULL;
// ***********

int server_network = -1; // server id

// providing list of user connect to server 
void sendListOfUser(int socket, ClientList *node)
{
   ClientList *curr = header;
   int count = 0;
   while (curr != NULL)
   {
      if (curr != node)
      {
         char buffer[buffer_size];
         snprintf(buffer, sizeof(buffer), " %d. reciver address %d", count + 1, curr->client_id);
         send(socket, buffer, sizeof(buffer), 0);
         count++;
      }

      curr = curr->next_user;
   }
   if (count == 0)
      sendMessage(socket, "no reciver found");
}
// broadcast a message to user
void broadCastToALLUser(char *message, ClientList *parent)
{
   ClientList *curr = header;
   while (curr != NULL)
   {
      if (curr != parent)
      {
         send(curr->client_id, message, strlen(message), 0);
      }
      curr = curr->next_user;
   }
}

// sending file
void fileTransfer(int client_socket, char *file_name)
{
   printf("---->begin file transfter to client %d\n", client_socket);
   int status = sendFile(file_name, client_socket);
   if (status < 0)
   {
      perror("file is send :");
      remove(file_name);
      return;
   }
   printf("---->file transfer ->sucess send to %d\n", client_socket);

   remove(file_name);
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
   long size_of_file;
   char buffer[buffer_size];
   char *dir_path = "."; // Current directory
   int file_count = getNumberofFileInFolder(dir_path);
   recv(network_socket, &size_of_file, sizeof(size_of_file), 0);
   printf("file size : %.4fKB\n", (size_of_file) / 1024.0);
   char file_name[buffer_size];
   snprintf(file_name, sizeof(file_name), "temp_%d.zip", file_count);

   FILE *file = fopen(file_name, "wb");
   long curr_size = 0;
   const char *data = "sending-file";
   char *message = (char *)malloc(strlen(data) + 1);
   strcpy(message, data);

   sendMessage(client_id, message);

   int i = 0;
   while (curr_size < size_of_file)
   {
      size_t byte_size = recv(network_socket, buffer, sizeof(buffer), 0);
      fwrite(buffer, sizeof(char), byte_size, file);
      curr_size += byte_size;
   }
   fclose(file);
   printf("file reach to sever -> sender %d\n", network_socket);
   fileTransfer(client_id, file_name);
   free(message);
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
      }
   }
   if (n == 0)
   {
      header = removeClient(header, curr_node);
      ClientList *dis_c_element = findClient(header, curr_node);

      if (dis_c_element == NULL)
         printf(" -- client disconnected %d -- \n", curr_node->client_id);

      ClientList *curr = header;
      while (curr != NULL)
      {

         char *disconnect_message = (char *)malloc(buffer_size);
         snprintf(disconnect_message, 50, "disconnect-user %d", client_id);
         sendMessage(curr->client_id, disconnect_message);
         free(disconnect_message);

         curr = curr->next_user;
      }
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
void sendDataToNewClient(int client_socket)
{

   ClientList *node = header;
   char message[buffer_size];
   while (node != NULL)
   {
      snprintf(message, buffer_size, "connected-user %d", node->client_id);
      send(client_socket, message, sizeof(message), 0);
      node = node->next_user;
   }
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
      char message[buffer_size];
      snprintf(message, buffer_size, "new-user %d", client_socket);
      broadCastToALLUser(message, NULL);
      sendDataToNewClient(client_socket);
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
