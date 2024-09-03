#include <netinet/in.h>
typedef struct ClientList
{
    int client_id;
    struct sockaddr_in *address;
    struct ClientList *next_user;
} ClientList;
ClientList *createClientNode(int id, struct sockaddr_in *addr);
void addNewClient(ClientList *header, ClientList *new_client);
ClientList *findClient(ClientList *header, ClientList* target_user);
ClientList *removeClient(ClientList *header, ClientList* disconnect_client);
