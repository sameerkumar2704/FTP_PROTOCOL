#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
int createTcpSocket();
struct sockaddr_in *createTcpIpV4SocketAddress(char *ip, int port_number);
int sendMessage(int id, char *message);
int recvMessage(int network_socket, char *buffer, size_t size);
