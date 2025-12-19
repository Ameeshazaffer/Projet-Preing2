#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char usine[64];
    char amont[64];
    char aval[64];
    float volume;
    float fuites;
}LigneCSV;

typedef struct{
    char* id;                 
    float capacite_max;       
    float volume_total_capte;    
    float volume_total_traite;       
}Usine;

typedef struct avl{
    Usine usine;
    int eq;                   
    struct avl *fg;
    struct avl *fd;
}AVL;

typedef Usine *pUsine;
typedef AVL *pAVL;

void ligneUsine(const LigneCSV* ligne,pAVL* a);
void ligneSourceUsine(const LigneCSV* ligne,pAVL* a);
int typeLigne(const char *usine,const char *amont,const char *aval,const char *volume,const char *fuites);
float conversion(const char *chaine);
void decouperLigne(FILE* fichierCSV,pAVL* a);
void traiterFichierCSV(FILE* f,pAVL a,int choix);
void creationFichierCSV(FILE* f,pAVL a,int choix);
void creationHistogramme(pAVL a ,int choix);
void libererAVL(pAVL a);
