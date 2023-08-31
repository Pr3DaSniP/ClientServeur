#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct communication
{
    int duree;
    int heure;
    int type;
    char date[11];
    char adresse_emetteur[16];
    char adresse_recepteur[16];
};

// Fonction pour sauvegarder les données dans un fichier log
void sauvegarde(struct communication c)
{
    FILE *fichier;
    fichier = fopen("communication.log", "a");
    if (fichier != NULL)
    {
        fprintf(fichier, "%s %d %d %d %s %s\n", c.date, c.heure, c.duree, c.type, c.adresse_emetteur, c.adresse_recepteur);
    }
    else
    {
        printf("Il y a eu une erreur lors de l'ouverture du fichier\n");
    }
    fclose(fichier);
}

// Fonction pour parser le fichier de log pour récupérer une communication par rapport à la date
struct communication recupere(char *date)
{
    FILE *fichier;
    struct communication c;
    fichier = fopen("communication.log", "r");
    if (fichier != NULL)
    {
        while (fscanf(fichier, "%s %d %d %d %s %s\n", c.date, &c.heure, &c.duree, &c.type, c.adresse_emetteur, c.adresse_recepteur) != EOF)
        {
            if (strncmp(c.date, date, 10) == 0)
            { // Je compare les 10 premiers caractère du contenu de temp et de date
                fclose(fichier);
                return c;
            }
        }
    }
    else
    {
        printf("Il y a une erreur lorque de l'ouverture du fichier\n");
    }
}

void creerCommunication()
{
    struct communication c;
    system("clear");
    printf("Saisissez sous la forme :\nduree, l'heure, type, la date, l'adresse IP de l'emetteur, l'adresse IP du recepteur\n");
    scanf("%d%d%d%s%s%s", &c.duree, &c.heure, &c.type, c.date, c.adresse_emetteur, c.adresse_recepteur);
    sauvegarde(c);
}

void afficheCommunication(struct communication c)
{
    system("clear");
    printf("============================================================\n");
    printf("Information concernant la communication :\n");
    printf("Duree : %d\nHeure : %d\nType : %d\nDate : %s\n", c.duree, c.heure, c.type, c.date);
    printf("Adresse emetteur : %s\nAdresse recepteur : %s\n", c.adresse_emetteur, c.adresse_recepteur);
    printf("============================================================\n");
}

int menu()
{
    int choix;
    printf("1. Générer une nouvelle communication\n");
    printf("2. Afficher les détails d'une communication\n");
    printf("3. Quitter\n");
    scanf("%d", &choix);
    return choix;
}

int main(int argc, char **argv)
{
    int choix;
    do
    {
        choix = menu();
        switch (choix)
        {
        case 1:
            creerCommunication();
            break;
        case 2:
            printf("Saisissez la date au format d/m/y :\n");
            char date[11];
            scanf("%s", date);
            struct communication com = recupere(date);
            afficheCommunication(com);
            break;
        }
    } while (choix != 3);

    return 0;
}