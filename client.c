#include <stdio.h>
#include "./headers/create_socket.h"
#include "string.h"
#include <pthread.h>
#include "./headers/file_zip.h"

#define PORT_NO 9002
#define buffer_size 1024
int network_socket = -1;
int selected_id = -1;
char dir[buffer_size];
char file_name[buffer_size];
enum
{
    SHOW_ALL_USERS,
    READ_TO_RECIVE_DATA,
    SERVER_SHUT_DOWN,
    SELECT_USER,
    ABORT_DATA_SENDING,
};
int findingCommandType(char *command)
{
    if (!strcmp(command, "show-all-users\n"))
        return SHOW_ALL_USERS;
    if (!strcmp(command, "file-reciver-on\n"))
        return READ_TO_RECIVE_DATA;
    if (!strncmp(command, "select-client", strlen("select-client")))
    {
        int i = 0;
        while (i < strlen(command))
        {
            if (command[i] == ' ')
                break;
            i++;
        }
        selected_id = 0;
        i++;
        while (i < strlen(command) - 1)
        {
            selected_id = selected_id * 10 + (command[i] - '0');
            i++;
        }
        return SELECT_USER;
    }

    return -1;
}
void zipe_file_to_send_server(char *sourc_file, char *file_name)
{
    zip_files("transfer_file.zip", sourc_file, file_name);
    printf(" file %s zip as transfer_file.zip\n", file_name);

    char server_message[1024];
    FILE *f = fopen("transfer_file.zip", "rb");
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    send(network_socket, &file_size, sizeof(file_size), 0);
    int size;
    while (1)
    {
        int size = fread(server_message, sizeof(char), sizeof(server_message), f);
        if (size == 0)
            break;
        int status = send(network_socket, server_message, size, 0);
        if (status == -1)
        {
            perror("Data not send :");
            return;
        }
        printf("%s\n", server_message);
    }
    printf("Data is send to server \n");
    fclose(f);
    remove("./transfer_file.zip");
}
void sendCommandToServer(int commandType)
{

    switch (commandType)
    {
    case SHOW_ALL_USERS:
        send(network_socket, &commandType, sizeof(commandType), 0);
        break;
    case READ_TO_RECIVE_DATA:
        send(network_socket, &commandType, sizeof(commandType), 0);
        break;
    case SELECT_USER:
        printf("enter path name : \n");
        fgets(dir, buffer_size, stdin);
        dir[strcspn(dir, "\n")] = '\0';
        printf("enter file name : \n");
        fgets(file_name, buffer_size, stdin);
        file_name[strcspn(file_name, "\n")] = '\0';
        send(network_socket, &commandType, sizeof(commandType), 0);
        break;
    default:
        printf("404 command not found \n");
        break;
    }
}
void fileReciver()
{
    long size_of_file = 0;
    char buffer[buffer_size];
    recv(network_socket, &size_of_file, sizeof(size_of_file), 0);
    printf("file size : %ld\n", size_of_file);
    FILE *file = fopen("./client_data/file.zip", "wb");
    long curr_size = 0;
    while (curr_size < size_of_file)
    {
        size_t byte_size = recv(network_socket, buffer, sizeof(buffer), 0);
        fwrite(buffer, sizeof(char), byte_size, file);
        curr_size += byte_size;
    }
    fclose(file);
    unzip_file("./result");
}
void closeSocket()
{
    shutdown(network_socket, SHUT_RDWR);
    close(network_socket);
}
void *serverResponseHandler()
{
    char buffer[buffer_size];
    int n = 0;
    while (1)
    {
        n = recv(network_socket, buffer, sizeof(buffer), 0);

        if (!strcmp("server-down", buffer) || n == 0)
        {
            printf("%s\n", buffer);
            closeSocket();
            break;
        }
        buffer[n] = '\0';

        if (!strcmp("provide-user-id", buffer))
        {
            printf("%d\n", selected_id);
            send(network_socket, &selected_id, sizeof(selected_id), 0);

            zipe_file_to_send_server(dir, file_name);
        }
        else
        {
            printf("server res : %s\n", buffer);
            if (!strncmp(buffer, "yes", strlen("yes")))
            {
                fileReciver();
            }
        }
    }
}
int createClient()
{
    network_socket = createTcpSocket();
    struct sockaddr_in *socket_address = createTcpIpV4SocketAddress("", PORT_NO);
    int connection_status = connect(network_socket, (struct sockaddr *)socket_address, sizeof(*socket_address));

    if (connection_status == -1)
    {
        printf("somethig went wrong\n");
        close(network_socket);
        return -1;
    }

    return 1;
}

int main()
{

    if (createClient() == -1)
    {
        printf("server is not connect \n");
        return -1;
    }
    pthread_t t1;
    pthread_create(&t1, NULL, &serverResponseHandler, NULL);
    char command[buffer_size];
    while (1)
    {
        fgets(command, buffer_size, stdin);

        if (!strcmp(command, "exit\n"))
        {
            break;
        }
        else
        {
            sendCommandToServer(findingCommandType(command));
        }
        memset(command, 0, sizeof(command));
    }
    closeSocket();
    return -1;
}
