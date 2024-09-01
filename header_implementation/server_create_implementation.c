#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "string.h"
#include <stdio.h>

int createTcpSocket()
{
    return socket(AF_INET, SOCK_STREAM, 0);
}
struct sockaddr_in *createTcpIpV4SocketAddress(char *ip, int port_number)
{
    struct sockaddr_in *socket_address = malloc(sizeof(struct sockaddr_in));
    socket_address->sin_family = AF_INET;
    socket_address->sin_port = htons(port_number);
    socket_address->sin_addr.s_addr = INADDR_ANY;

    return socket_address;
}
int sendMessage(int id, char *message)
{
    int length = strlen(message);
    return send(id, message, length, 0);
}
int recvMessage(int id, char *buffer, size_t buffer_size)
{
    return recv(id, buffer, 1024, 0);
}
