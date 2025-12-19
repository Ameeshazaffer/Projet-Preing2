#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "histogramme.h"
#include "leaks.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Erreur : arguments insuffisants\n");
        fprintf(stderr, "Usage : %s <fichier> <histo|leaks> [option]\n", argv[0]);
        return EXIT_FAILURE;
    }
    char *nom_fichier = argv[1];
    char *commande = argv[2];
    FILE *f = fopen(nom_fichier, "r");
    if (f == NULL) {
        perror("Erreur ouverture fichier");
        return EXIT_FAILURE;
    }
    if (strcmp(commande, "histo") == 0) {
        if (argc != 4) {
            fprintf(stderr, "Usage : %s <fichier> histo <max|src|real>\n", argv[0]);
            fclose(f);
            return EXIT_FAILURE;
        }
        int choix;
        if (strcmp(argv[3], "max") == 0)
            choix = 1;
        else if (strcmp(argv[3], "src") == 0)
            choix = 2;
        else if (strcmp(argv[3], "real") == 0)
            choix = 3;
        else {
            fprintf(stderr, "Erreur : type d’histogramme invalide\n");
            fclose(f);
            return EXIT_FAILURE;
        }
        pAVL avl = NULL;
        decouperLigne(f, &avl);
        creationHistogramme(avl, choix);
        libererAVL(avl);
    }else if (strcmp(commande, "leaks") == 0) {
        if (argc != 4) {
            fprintf(stderr, "Usage : %s <fichier> leaks <identifiant>\n", argv[0]);
            fclose(f);
            return EXIT_FAILURE;
        }
        char *id_usine = argv[3];
        pAVL_sup avl = NULL;
        construire_arbre(f, &avl);
        float pertes = calcul_final_pertes(avl, id_usine);
        if (pertes < 0) {
            printf("-1\n");
        } else {
            printf("Pertes pour l'usine %s : %.3f M.m3\n", id_usine, pertes);
            ajout_resultat_fichier(id_usine, pertes);
        }
        liberer_memoire(avl);
    }else {
        fprintf(stderr, "Erreur : commande inconnue '%s'\n", commande);
        fclose(f);
        return EXIT_FAILURE;
    }
    fclose(f);
    return EXIT_SUCCESS;
}
