#include <stdlib.h>
#include <stdio.h>
#include "../headers/store_user.h"
#include <netinet/in.h>
ClientList *createClientNode(int id, struct sockaddr_in *addr)
{
    ClientList *node = malloc(sizeof(ClientList));
    node->client_id = id;
    node->address = addr;
    node->next_user = NULL;
    return node;
}
void addNewClient(ClientList *header, ClientList *new_client)
{
    if (header == NULL)
    {
        printf("Header is point to NULL\n");
        return;
    }

    ClientList *curr = header;
    ClientList *prev = NULL;
    while (curr != NULL)
    {
        prev = curr;
        curr = curr->next_user;
    }

    prev->next_user = new_client;
}
ClientList *findClient(ClientList *header, int target_user)
{
    ClientList *curr = header;
    while (curr != NULL)
    {
        if (curr->client_id == target_user)
            return curr;
        curr = curr->next_user;
    }
    return NULL;
}
ClientList *removeClient(ClientList *header, int disconnect_client)
{
    ClientList *curr = header;
    ClientList *prev = NULL;
    while (curr != NULL)
    {
        if (curr->client_id == disconnect_client)
        {
            break;
        }
        prev = curr;
        curr = curr->next_user;
    }
    if (prev == NULL)
        return curr->next_user;

    prev->next_user = curr->next_user;
    return header;
}
