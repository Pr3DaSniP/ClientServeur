#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define S_PORT 7778

int main()
{
    char buffer[1024];
    int sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd == -1)
    {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(S_PORT);
    int bnd = bind(sd, (struct sockaddr *)&server, sizeof(server));
    if (bnd == -1)
    {
        perror("bind");
        exit(1);
    }
    struct sockaddr_in client;
    int client_size = sizeof(struct sockaddr_in);
    for (int i = 0; i < 100; i++)
    {
        printf("Waiting for message from client...\n");
        recvfrom(sd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client, &client_size);
        printf("[CLIENT]: %s\n", buffer);
    }
    close(sd);
}