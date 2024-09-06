
#include "./client_interface.h"


#define PORT_NO 9002
#define buffer_size 1024
// defining static data

int network_socket = -1;
int selected_id = -1;
char dir[buffer_size];
char file_name[buffer_size];

// ************




int get_UserId(char *command)
{
    int digit = 0;
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
        digit = digit * 10 + (command[i] - '0');
        i++;
    }
    return digit;
}
int findingCommandType(char *command)
{
    if (!strcmp(command, "show-all-users\n"))
        return SHOW_ALL_USERS;
    if (!strcmp(command, "file-reciver-on\n"))
        return READ_TO_RECIVE_DATA;
    if (!strncmp(command, "select-client", strlen("select-client")))
    {
        selected_id = get_UserId(command);
        return SELECT_USER;
    }

    return -1;
}
void zip_file_to_send_server(char *sourc_file, char *file_name)
{
    // compressing file before transfer
    zip_files("transfer_file.zip", sourc_file, file_name);
    printf("--> %s is zipped as transfer_file.zip\n", file_name);
    // calling custom functions from reuse_func.h
    int status = sendFile("./transfer_file.zip", network_socket);
    if (status < 0)
    {
        perror("file not send to server : ");
        remove("./transfer_file.zip");
        return;
    }
    printf("--> transfer_file.zip is send to server \n");
    remove("./transfer_file.zip");
}
void get_detail_of_file_and_path_name(int commandType)
{
    printf("enter path name : \n");
    fgets(dir, buffer_size, stdin);
    dir[strcspn(dir, "\n")] = '\0';
    printf("enter file name : \n");
    fgets(file_name, buffer_size, stdin);
    file_name[strcspn(file_name, "\n")] = '\0';
    send(network_socket, &commandType, sizeof(commandType), 0);
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
        get_detail_of_file_and_path_name(commandType);
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

    printf("file size : %.4fKB\n", (size_of_file) / 1024.0);
    char *dir_path = "./client_data"; // Current directory
    int file_count = getNumberofFileInFolder(dir_path);
    char file_name[1024];
    snprintf(file_name, sizeof(file_name), "./client_data/file_%d.zip", file_count);
    FILE *file = fopen(file_name, "wb");
    long curr_size = 0;
    while (curr_size < size_of_file)
    {
        size_t byte_size = recv(network_socket, buffer, sizeof(buffer), 0);
        fwrite(buffer, sizeof(char), byte_size, file);
        curr_size += byte_size;
    }
    fclose(file);
    unzip_file("./result", file_name);
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
        if (n < 0)
        {
            perror("recv prot : ");
            break;
        }

        if (!strcmp("server-down", buffer) || n == 0)
        {
            printf("----->%s\n ", buffer);
            closeSocket();
            break;
        }
        buffer[n] = '\0';

        if (!strcmp("provide-user-id", buffer))
        {
            send(network_socket, &selected_id, sizeof(selected_id), 0);

            zip_file_to_send_server(dir, file_name);
        }
        else
        {
            printf("server res :\n----> %s\n", buffer);
            if (!strcmp(buffer, "sending-file"))
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
        perror("server error  :");
        close(network_socket);
        return -1;
    }
    printf("---> client connect to server at : port %d\n", PORT_NO);

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
