#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "histogramme.h"
#include "leaks.h"

int main(int argc, char **argv) {

    if (argc < 4) { // Vérification des arguments
        printf("Usage : %s <fichier> <histo|leaks> <option>\n", argv[0]);
        return 1;
    }

    char *fichier = argv[1];
    char *commande = argv[2];
    char *option = argv[3];

    FILE *f = fopen(fichier, "r"); // ouverture du fichier
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier\n");
        return 1;
    }

    //HISTOGRAMME
    if (strcmp(commande, "histo") == 0) {

        int choix = 0;
        if (strcmp(option, "max") == 0)
            choix = 1; // histogramme des capacités max
        else if (strcmp(option, "src") == 0)
            choix = 2; // histogramme des volumes captés
        else if (strcmp(option, "real") == 0)
            choix = 3; // histogramme des volumes réellement traités
        else {
            printf("Type d'histogramme invalide\n");
            fclose(f);
            return 1;
        }

        pAVL avl = NULL; // créer l'avl
        decouperLigne(f, &avl);

        if (avl == NULL) {
            printf("Aucune donnee trouvee\n");
            fclose(f);
            return 1;
        }

        creationHistogramme(avl, choix);
        libererAVL(avl); // libérer avl
    }

    // LEAKS
    else if (strcmp(commande, "leaks") == 0) {

        pAVL_sup avl = NULL;
        construire_arbre(f, &avl); // construction avl

        float pertes = calcul_final_pertes(avl, option); // calcul de la perte d'eau de l'usine

        if (pertes < 0) {
            printf("-1\n"); // affiche -1 si l'usine n'a pas été trouvé
        } else {
            printf("Pertes pour %s : %.3f M.m3\n", option, pertes); // affiche le volume de pertes de l'usine
            ajout_resultat_fichier(option, pertes);
        }

        liberer_memoire(avl); // libère mémoire de l'arbre
    }

    else {
        printf("Commande inconnue\n");
        fclose(f);
        return 1;
    }

    fclose(f);
    return 0;
}
