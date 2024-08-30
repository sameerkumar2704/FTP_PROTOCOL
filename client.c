#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "./headers/create_socket.h"

#define PORT_NO 9002
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

    FILE *file = fopen("./client_data/file.zip", "wb");

    char message[1024];
    int n = 0;
    while ((n = recv(network_socket, message, sizeof(message), 0)) > 0)
    {
        fwrite(message, sizeof(char), n, file);
        printf("%s\n", message);
    }

    fclose(file);
    close(network_socket);

    return 1;
}
int main()
{
    createClient();
    return -1;
}
