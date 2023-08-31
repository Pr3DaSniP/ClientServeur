#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define S_PORT 7778

// Récupération du texte dans le fichier
char *getTexte(char *filename)
{
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }
    char *texte = malloc(1000);
    char c = fgetc(fptr);
    int i = 0;
    while (c != EOF)
    {
        texte[i] = c;
        c = fgetc(fptr);
        i++;
    }
    fclose(fptr);
    return texte;
}

// Retourne le minimum de entre 3 entiers
int min(int a, int b, int c)
{
    int min = a;
    if (b < min)
    {
        min = b;
    }
    if (c < min)
    {
        min = c;
    }
    return min;
}

// Calcul de la distance de Levenshtein
int DistanceLevenshtein(char *source, char *target)
{
    int len1 = strlen(source);
    int len2 = strlen(target);
    int D[len1 + 1][len2 + 1];
    int cost = 0;

    for (int i = 0; i <= len1; i++)
    {
        D[i][0] = i;
    }
    for (int j = 0; j <= len2; j++)
    {
        D[0][j] = j;
    }

    for (int i = 1; i <= len1; i++)
    {
        for (int j = 1; j <= len2; j++)
        {
            if (source[i - 1] == target[j - 1])
            {
                cost = 0;
            }
            else
            {
                cost = 1;
            }
            D[i][j] = min(
                D[i - 1][j] + 1,
                D[i][j - 1] + 1,
                D[i - 1][j - 1] + cost);
        }
    }
    return D[len1][len2];
}

float pourcentageSimilitude(char *source, char *target)
{
    if (source == NULL || target == NULL)
    {
        return 0.0;
    }
    if (strlen(source) == 0 || strlen(target) == 0)
    {
        return 0.0;
    }
    if (strcmp(source, target) == 0)
    {
        return 100;
    }
    int distance = DistanceLevenshtein(source, target);
    int maxL = strlen(source) > strlen(target) ? strlen(source) : strlen(target);
    return (1.0 - (distance / (double)maxL)) * 100;
}

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

    // Récupération du texte dans le fichier database.txt
    char *texte = getTexte("database.txt");

    for (int i = 0; i < 100; i++)
    {
        bzero(buffer, sizeof(buffer));
        printf("Waiting for message from client...\n");
        printf("//////////////////////////////////////////////\n");
        printf("Texte: %s\n", texte);
        printf("//////////////////////////////////////////////\n");
        recvfrom(sd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client, &client_size);
        buffer[strlen(buffer) - 1] = '\0';
        printf("[CLIENT]: %s\n", buffer);
        printf("//////////////////////////////////////////////\n");
        // Calcul du pourcentage de similitude
        float pourcentage = pourcentageSimilitude(buffer, texte);
        printf("Pourcentage de similitude: %f%%\n", pourcentage);
        printf("//////////////////////////////////////////////\n");

        // Envoi du pourcentage de similitude au client
        sendto(sd, &pourcentage, sizeof(pourcentage), 0, (struct sockaddr *)&client, client_size);
    }
    close(sd);
}