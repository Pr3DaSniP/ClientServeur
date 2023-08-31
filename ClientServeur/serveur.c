#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));

    int welcomeSocket, newSocket;
    char buffer[1024];
    int reponse, a, b;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;

    serverAddr.sin_port = htons(7891);

    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    bind(welcomeSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    while (listen(welcomeSocket, 5) == 0)
    {
        printf("\nWaiting for connection\n");
        addr_size = sizeof serverStorage;
        newSocket = accept(welcomeSocket, (struct sockaddr *)&serverStorage, &addr_size);

        do
        {
            // Envoie Captcha arithmétique
            a = rand() % 10;
            b = rand() % 10;
            sprintf(buffer, "%d + %d = ", a, b);
            send(newSocket, buffer, sizeof(buffer), 0);

            printf("\nWaiting for response\n");

            // Nettoyage du buffer
            memset(buffer, '\0', sizeof(buffer));

            // Récupération de la réponse
            recv(newSocket, buffer, sizeof(buffer), 0);
            reponse = atoi(buffer);

            // Vérification de la réponse
            if (reponse == (a + b))
            {
                send(newSocket, "OK", 2, 0);
            }
            else
            {
                send(newSocket, "KO", 2, 0);
            }
        } while (reponse != (a + b));
    }
    return 0;
}