#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Noeud Noeud;
typedef struct Liste Liste; 

typedef struct Noeud{
	char* id; // identifiants du distributeurs d'eau
	float volume_initial;
	Liste* enfants;
}Noeud;

typedef struct Noeud* pNoeud; 

typedef struct Liste{
	Noeud* enfant; // en aval
	float fuites_pourcentage; // pourcentage de fuites 
	struct Liste* suivant; //
}Liste; 

typedef struct Liste* pListe; 


typedef struct avl_sup{
	char* id;
    pNoeud noeud;
    int eq;                   
    struct avl_sup *fg;
    struct avl_sup *fd;
}AVL_sup;

typedef AVL_sup* pAVL_sup;


typedef struct {
    char usine[64];
    char amont[64];
    char aval[64];
    float volume;
    float coeff;
} LigneCSV_sup;

pNoeud creerNoeud(const char* identifiant);
pListe creerenfant(pNoeud aval, float fuite);
void ajouterenfant(pNoeud parent, pNoeud enfant, float fuites);
pAVL_sup creerAVL_sup(const char* id, pNoeud noeud);
int min_sup(int a, int b);
int max_sup(int a, int b);
int min3_sup(int a, int b, int c);
int max3_sup(int a, int b, int c);
pAVL_sup rotationDroite_sup(pAVL_sup a);
pAVL_sup rotationGauche_sup(pAVL_sup a);
pAVL_sup doubleRotationGauche_sup(pAVL_sup a);
pAVL_sup doubleRotationDroite_sup(pAVL_sup a);
pAVL_sup equilibrerAVL_sup(pAVL_sup a);
pNoeud rechercheAVL_sup(pAVL_sup a,const char* id);
pAVL_sup insertionAVL_sup(pAVL_sup a, const char* id, pNoeud n, int* h);
pNoeud obtenirnoeud(pAVL_sup* avl, const char* id);
void traiter_une_ligne(LigneCSV_sup* l, pAVL_sup* a);
int lireLigne(FILE* f, LigneCSV_sup* l);
void construire_arbre(FILE* f, pAVL_sup* a);
pNoeud verification_identifiant(pAVL_sup avl, const char* id);
int compter_enfants(pNoeud noeud);
float calcul_pertes(pNoeud noeud, float volume);
void ajout_resultat_fichier(const char* identifiant, float volume_pertes);
float calcul_final_pertes(pAVL_sup avl, const char* identifiant_usine);
void liberer_noeud(pNoeud n);
void liberer_memoire(pAVL_sup a);
