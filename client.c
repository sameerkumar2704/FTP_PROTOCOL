#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "./headers/create_socket.h"
#include "string.h"

#define PORT_NO 9002
#define buffer_size 1024
int createClient()
{
    int network_socket = createTcpSocket();
    struct sockaddr_in *socket_address = createTcpIpV4SocketAddress("", PORT_NO);
    int connection_status = connect(network_socket, (struct sockaddr *)socket_address, sizeof(*socket_address));

    if (connection_status == -1)
    {
        printf("somethig went wrong\n");
        close(network_socket);
        return -1;
    }

    // FILE *file = fopen("./client_data/file.zip", "wb");
    char buffer[buffer_size];
    int n = 0;
    int recived_message_len = recv(network_socket, buffer, buffer_size - 1, 0);
    buffer[recived_message_len] = '\0';
    printf("your data :%s \n", buffer);
    buffer[recived_message_len] = 0;
    while ((n = recv(network_socket, buffer, buffer_size - 1, 0)) > 0)
    {

        // fwrite(message, sizeof(char), n, file);
        printf("%s %d\n", buffer, n);
    }

    // fclose(file);
    shutdown(network_socket, SHUT_RDWR);
    close(network_socket);

    return 1;
}
int main()
{
    createClient();
    return -1;
}
