#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
enum
{
    SHOW_ALL_USERS,
    READ_TO_RECIVE_DATA,
    SERVER_SHUT_DOWN,
    SELECT_USER,
    ABORT_DATA_SENDING,
};

int createTcpSocket();
struct sockaddr_in *createTcpIpV4SocketAddress(char *ip, int port_number);
int sendMessage(int id, char *message);
int recvMessage(int network_socket, char *buffer, size_t size);
long recvInteger(int socket_id );
long sendInteger(int socket_id , long value );