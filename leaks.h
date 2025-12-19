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


typedef struct avl{
	char* id;
    pNoeud noeud;
    int eq;                   
    struct avl *fg;
    struct avl *fd;
}AVL_sup;

typedef AVL_sup* pAVL_sup;


typedef struct {
    char usine[50];
    char amont[50];
    char aval[50];
    float volume;
    float coeff;
} LigneCSV;

creerNoeud(const char* identifiant);
pListe creerenfant(pNoeud aval, float fuite);
pListe creerenfant(pNoeud aval, float fuite);
void ajouterenfant(pNoeud parent, pNoeud enfant, float fuites);
pAVL_sup creerAVL_sup(const char* id, pNoeud noeud);
int min(int a, int b);
int max(int a, int b);
int min3(int a, int b, int c);
int max3(int a, int b, int c);
pAVL_sup rotationDroite(pAVL_sup a);
pAVL_sup rotationGauche(pAVL_sup a);
pAVL_sup doubleRotationGauche(pAVL_sup a);
pAVL_sup doubleRotationDroite(pAVL_sup a);
pAVL_sup equilibrerAVL(pAVL_sup a);
pNoeud rechercheAVL(pAVL_sup a,const char* id);
pAVL_sup insertionAVL(pAVL_sup a, const char* id, pNoeud n, int* h);
pNoeud obtenirnoeud(pAVL_sup* avl, const char* id);
void traiter_une_ligne(LigneCSV* l, pAVL_sup* a);
int lireLigne(FILE* f, LigneCSV* l);
void construire_arbre(FILE* f, pAVL_sup* a)
pNoeud vérification_identifiant(pAVL_sup avl, const char* id);
int compter_enfants(pNoeud noeud);
float calcul_pertes(pNoeud noeud, float volume);
void ajout_résulat_fichier(const char* identifiant, float volume_pertes);
float calcul_final_pertes(pAVL_sup avl, const char* identifiant_usine);
void liberer_noeud(pNoeud n);
void liberer_memoire(pAVL_sup a);
