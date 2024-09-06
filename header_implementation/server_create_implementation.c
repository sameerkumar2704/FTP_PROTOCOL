#include "../headers/create_socket.h"

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
    return recv(id, buffer, buffer_size , 0);
}
long recvInteger(int socket_id  ){
    long data ;
    int status = recv(socket_id, &data, sizeof(data), 0);
    if(status == -1){
        return -1;
    }
    return data;
}

long sendInteger(int socket_id , long value ){
    long data = value;
    int status = send(socket_id, &data, sizeof(data), 0);
    if(status == -1){
        return -1;
    }
    return 1;
}
