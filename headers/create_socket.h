int createTcpSocket();
struct sockaddr_in *createTcpIpV4SocketAddress(char *ip, int port_number);
int sendMessage(int id, char *message);
int recvMessage(int network_socket, char *buffer, size_t size);
