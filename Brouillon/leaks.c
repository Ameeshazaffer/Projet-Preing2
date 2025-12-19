                                                                      // toutes les fonctions pour afficher la perte d'eau d'une usine //


                                                                                           // structures //


                                                                                        // partie 1 : fonctions qui crées un arbre classique //

// fonction qui crée unn noeud

pNoeud creerNoeud(const char* identifiant){ 
	pNoeud noeud=malloc(sizeof(Noeud));
	if(noeud==NULL){
		return NULL;
	}
	noeud->id=malloc(strlen(identifiant)+1); // faire de la mémoire pour la chaîne de carcatère de l'identifiant
	if(noeud->id==NULL){
		free(noeud); // libérer mémoire allouée du noeud 
		return NULL;
	}
	strcpy(noeud->id, identifiant);
	noeud->volume_initial=0;
	noeud->enfants=NULL; //(initialiser la liste d'enfant à NULL)
	return noeud;
}


// fonction qui crée un enfant de la liste 

pListe creerenfant(pNoeud aval, float fuite){
	pListe a= malloc(sizeof(Liste));
	if(a==NULL){
		return NULL;
	}
	a->enfant=aval;
	a->fuites_pourcentage=fuite;
	a->suivant=NULL;
	return a;
}



// fonction qui ajoute l'enfant

void ajouterenfant(pNoeud parent, pNoeud enfant, float fuites){
	pListe a=creerenfant(enfant, fuites);
	if (a == NULL){
		return;
	}
	a->suivant=parent->enfants; 
	parent->enfants=a;
}

// fonction qui crée un AVL supplémentaire

pAVL_sup creerAVL_sup(const char* id, pNoeud noeud){ 
    pAVL_sup n = malloc(sizeof(AVL_sup));
    if (n == NULL) {
        return NULL;
    }
    n->id = malloc(strlen(id) + 1);
	if (n->id==NULL){
		free(n);
		return NULL;
	}
    strcpy(n->id, id);
	n->noeud= noeud;
    n->eq = 0;
    n->fg = NULL;
    n->fd = NULL;
    return n;
}


// fonction qui recherche un noeud avec l'avl - retourne null ou neoud 
int min(int a, int b){
    if(a < b){
        return a;
    }else{
        return b;
    }    
}

int max(int a, int b){
    if(a > b){
        return a;
    }else{
        return b;
    }    
}

int min3(int a, int b, int c){
  return min(min(a,b),c);
}

int max3(int a, int b, int c){
  return max(max(a,b),c);
}


pAVL_sup rotationDroite(pAVL_sup a){
    pAVL_sup pivot = a->fg;
    int eq_a = a->eq;
    int eq_p = pivot->eq;
    a->fg = pivot->fd;
    pivot->fd = a;
    a->eq = eq_a - min(eq_p, 0) + 1;
    pivot->eq = max3(eq_a + 2, eq_a + eq_p + 2, eq_p + 1);
    return pivot;
}

pAVL_sup rotationGauche(pAVL_sup a){
    pAVL_sup pivot = a->fd;
    int eq_a = a->eq;
    int eq_p = pivot->eq;
    a->fd = pivot->fg;
    pivot->fg = a;
    a->eq = eq_a - max(eq_p, 0) - 1;
    pivot->eq = min3(eq_a - 2, eq_a + eq_p - 2, eq_p - 1);
    return pivot;
}

pAVL_sup doubleRotationGauche(pAVL_sup a){
    a->fd = rotationDroite(a->fd);
    return rotationGauche(a);
}

pAVL_sup doubleRotationDroite(pAVL_sup a){
    a->fg = rotationGauche(a->fg);
    return rotationDroite(a);
}


pAVL_sup equilibrerAVL(pAVL_sup a){
    if (a->eq >= 2){
        if (a->fd->eq >= 0){
            return rotationGauche(a);
        }else{
            return doubleRotationGauche(a);
        }
    }else if (a->eq <= -2){
        if(a->fg->eq <= 0){
            return rotationDroite(a);
        }else{
            return doubleRotationDroite(a);
        }
    }
    return a;
}

pNoeud rechercheAVL(pAVL_sup a,const char* id){
    if (a == NULL){
        return NULL;
    }
    if(strcmp(id, a->id) == 0){
        return a->noeud;
    }else if(strcmp(id, a->id) < 0){
        return rechercheAVL(a->fg, id);
    }else{
        return rechercheAVL(a->fd, id);
    }
}

// fonction qui insère noeud dans l'avl 

pAVL_sup insertionAVL(pAVL_sup a, const char* id, pNoeud n, int* h){
    if (a == NULL) {
        *h = 1;
        return creerAVL_sup(id, n);
    }
    
    if (strcmp(id,a->id) < 0){
        a->fg = insertionAVL(a->fg, id, n, h);
        *h = -*h;
    }
	else if(strcmp(id, a->id) > 0){
        a->fd = insertionAVL(a->fd, id, n, h);
    }
	else{
        *h = 0;
        return a; 
    }

    if(*h != 0){
        a->eq += *h;
        if(a->eq == 0){
            *h = 0;
        }else{
            *h = 1;
        }
    }

    return equilibrerAVL(a);
}



// fonction qui reagrde si le noeud existe sinon crée un noeud et insère dans l'avl - retourner neoud 

pNoeud obtenirnoeud(pAVL_sup* avl, const char* id){
	pNoeud noeud = rechercheAVL(*avl, id);
	if ( noeud != NULL ){ // si existe, retourner le noeud 
		return noeud; 
	}
	else{
		pNoeud noeud = creerNoeud(id);
		if(noeud==NULL){
			return NULL;
		}
		int h=0; 
		*avl=insertionAVL(*avl, id, noeud, &h); // on met pointeur pour que ça modifie dans tout le programme 
		return noeud; // retourne adresse du noeud créé
	}
}
		

// fonction qui traite une ligne du fichier en fonction du type de distributions faites 

void traiter_une_ligne(LigneCSV* l, pAVL_sup* a){

// source -> usine 
	
	if(strcmp(l->usine, "-")==0 && strcmp(l->amont,"-")!=0 && strcmp(l->aval, "-")!=0 && ( l->volume>0 )){
		pNoeud usine=obtenirnoeud(a, l->aval);

		float volume_après_fuites = l->volume*(1-(l->coeff/100.0));
		usine->volume_initial += volume_après_fuites; // car plusieurs sources peuvent rentrer dans une usine 

	}
// usine 

	if(strcmp(l->usine, "-")==0 && strcmp(l->amont,"-")!=0 && strcmp(l->aval, "-")==0 && ( l->volume>0 )){
		pNoeud usine=obtenirnoeud(a, l->amont);


	}
// usine->stockage & stockage->jonction & jonction->raccordement & raccordement->usager

	if(strcmp(l->amont,"-")!=0 && strcmp(l->aval,"-")!=0 && l->volume < 0 && l->coeff >= 0){
		pNoeud noeud_amont=obtenirnoeud(a, l->amont);
		pNoeud noeud_aval=obtenirnoeud(a, l->aval);

		ajouterenfant(noeud_amont, noeud_aval, l->coeff);
	}

}

// fonction qui lit la ligne et vérifie que c'est bien valide

int lireLigne(FILE* f, LigneCSV* l){
    char vol[50], coef[50];

    int lecture = fscanf(f, " %49[^;];%49[^;];%49[^;];%49[^;];%49[^\n]\n",l->usine, l->amont, l->aval, vol, coef);
    if(lecture != 5){
		return 0;
	}
	
    if(strcmp(vol, "-")==0){
		l->volume=-1.0f;
	}
	else{
		l->volume=atof(vol);
	}

	if(strcmp(coef,"-")==0){
		l->coeff=-1.0f;
	}
	else{
		l->coeff=atof(coef);
	}
    return 1;
} 


// fonction qui construit l'arbre 
void construire_arbre(FILE* f, pAVL_sup* a){
	LigneCSV l;
	while(lireLigne(f, &l)){
		traiter_une_ligne(&l, a);
	}
}



                            // partie 2 : fonctions qui calculent la perte d'eau d'une seule usine ( compte le nombre d'enfants + répartition volumes + addition pertes + créer le fichier et ajoute ligne dedans ) //

// fonction qui vérifie l'identifiant donnée

pNoeud vérification_identifiant(pAVL_sup avl, const char* id){
	return rechercheAVL(avl, id);
}

// fonction qui compte les enfants du noeud 
int compter_enfants(pNoeud noeud){
	int count=0;
	pListe actuel= noeud->enfants;
	while (actuel != NULL){
		count ++;
		actuel=actuel->suivant; 
	}
	return count;
}

// fcontion qui calcule la perte d'eau 
float calcul_pertes(pNoeud noeud, float volume){
	float pertes = 0.0;
	float volume_par_enfant, fuites, volume_apres_fuites;

	int nombre_enfants= compter_enfants(noeud); 

	if (noeud->enfants==NULL){
		return 0.0; 
	}
	volume_par_enfant=volume/nombre_enfants;

	pListe actuel= noeud->enfants; 
	while(actuel!=NULL){
		fuites=actuel->fuites_pourcentage/100.0;
		volume_apres_fuites=volume_par_enfant*(1-fuites);
		pertes += volume_par_enfant - volume_apres_fuites;

		pertes += calcul_pertes(actuel->enfant, volume_apres_fuites);
		actuel=actuel->suivant; 

	}
	return pertes; 
}



// fonction qui crée et rajoute le calcul dans un fichier

void ajout_resultat_fichier(const char* identifiant, float volume_pertes){
	FILE* fichier = fopen("leaks.dat", "a");
	if(fichier == NULL){
		exit(1);
	}
	fprintf(fichier, "%s;%.3fk.m3\n", identifiant, volume_pertes);
	fclose(fichier);
}

// fonction qui convertit en millions de m³

float calcul_final_pertes(pAVL_sup avl, const char* identifiant_usine){

	pNoeud usine = verification_identifiant(avl, identifiant_usine); // vérifie si identifiant bon
	if(usine==NULL){
		return -1.0f;
	}
	float pertes = calcul_pertes(usine, usine->volume_initial); // calcul pertes de l'usine
	float volume_pertes = pertes/1000.0f; // conversion de la perte en milllions.m^3
	return volume_pertes; 
}

// fonctions qui libère la mémoire

void liberer_noeud(pNoeud n){
    if (n == NULL) return;

    pListe l = n->enfants;
    while (l != NULL){
        pListe tmp = l;
        l = l->suivant;
        free(tmp);
    }

    free(n->id);
    free(n);
} 
void liberer_memoire(pAVL_sup a){
    if (a == NULL) return;

    liberer_memoire(a->fg);
    liberer_memoire(a->fd);

    liberer_noeud(a->noeud);

    free(a->id);
    free(a);
}
                                                       // ce qu'il faut mettre dans le programme principal //


int main (){
	FILE* fichier = NULL;  // ouverture du fichier c-wildwater_v0.dat en mode lecture et écriture
	fichier = fopen("c-wildwater_v0.dat", "r+");
	if (fichier == NULL){
		printf("Ouverture du fichier impossible\n");
		exit(1);
	}

	pAVL_sup avl= NULL; 
	construire_arbre(fichier, &avl);
	fclose(fichier);
	
	char id_usine[50];
	printf("Entrer l'identifiant de l'usine :\n");
	fgets(id_usine, 50, stdin);
    id_usine[strcspn(id_usine, "\n")] = 0;

	pNoeud usine = verification_identifiant(avl, id_usine); // vérifie si identifiant bon
	if(usine==NULL){
		printf("-1\n");
	}
	else{
		float pertes=calcul_final_pertes(avl, id_usine);
		printf("Le volume d'eau perdue par l'usine, dont l'identifiant est %49s, est de %3f millions de m^3\n", id_usine, pertes);
		ajout_resultat_fichier(id_usine,pertes); // informations du calcul mis dans le fichier leaks.dat
	}


	liberer_memoire(avl); // libérer avl et arbre
	return 0; 
}

	
