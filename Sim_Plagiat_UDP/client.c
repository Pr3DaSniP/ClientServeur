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
    int server_socket;
    struct sockaddr_in server_address;
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1)
    {
        perror("socket");
        exit(1);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(S_PORT);

    char buffer[1024];
    bzero(buffer, 1024);

    printf("Enter the message: ");
    fgets(buffer, 1024, stdin);
    sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&server_address, sizeof(server_address));

    // Récupération de la réponse
    float pourcentage;
    recvfrom(server_socket, &pourcentage, sizeof(pourcentage), 0, NULL, NULL);
    printf("Pourcentage de similitude: %f%%\n", pourcentage);
}