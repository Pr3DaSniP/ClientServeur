#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main()
{
    int clientSocket;
    char buffer[1024], reponseServeur[2];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;

    serverAddr.sin_port = htons(7891);

    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    addr_size = sizeof serverAddr;
    connect(clientSocket, (struct sockaddr *)&serverAddr, addr_size);

    do
    {
        recv(clientSocket, buffer, 1024, 0);

        printf("[CAPTCHA] : %s", buffer);
        // Envoie de la réponse
        int reponse;
        scanf("%d", &reponse);
        sprintf(buffer, "%d", reponse);
        send(clientSocket, buffer, sizeof(buffer), 0);

        // Nettoyage du buffer
        memset(buffer, '\0', sizeof(buffer));

        // Récupération de la réponse
        recv(clientSocket, reponseServeur, sizeof(reponseServeur), 0);
        printf("[REPONSE] : %s\n", reponseServeur);

    } while (strcmp(reponseServeur, "OK") != 0);

    // Deconnexion
    close(clientSocket);

    return 0;
}