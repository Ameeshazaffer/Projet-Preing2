
int min(int a,int b){
    if(a < b){
        return a;
    }else{
        return b;
    }    
}

int max(int a,int b){
    if(a > b){
        return a;
    }else{
        return b;
    }    
}

int min3(int a,int b,int c){
  return min(min(a,b),c);
}

int max3(int a,int b,int c){
  return max(max(a,b),c);
}


pAVL rotationDroite(pAVL a){
    pAVL pivot = a->fg;
    int eq_a = a->eq;
    int eq_p = pivot->eq;
    a->fg = pivot->fd;
    pivot->fd = a;
    a->eq = eq_a - min(eq_p, 0) + 1;
    pivot->eq = max3(eq_a + 2, eq_a + eq_p + 2, eq_p + 1);
    return pivot;
}

pAVL rotationGauche(pAVL a){
    pAVL pivot = a->fd;
    int eq_a = a->eq;
    int eq_p = pivot->eq;
    a->fd = pivot->fg;
    pivot->fg = a;
    a->eq = eq_a - max(eq_p, 0) - 1;
    pivot->eq = min3(eq_a - 2, eq_a + eq_p - 2, eq_p - 1);
    return pivot;
}

pAVL doubleRotationGauche(pAVL a){
    a->fd = rotationDroite(a->fd);
    return rotationGauche(a);
}

pAVL doubleRotationDroite(pAVL a){
    a->fg = rotationGauche(a->fg);
    return rotationDroite(a);
}


pAVL equilibrerAVL(pAVL a){
    if (a->eq >= 2){
        if (a->fd->eq >= 0){
            return rotationGauche(a);
        }else{
            return doubleRotationGauche(a);
        }
    }else if (a->eq <=-2){
        if(a->fg->eq <= 0){
            return rotationDroite(a);
        }else{
            return doubleRotationDroite(a);
        }
    }
    return a;
}


pAVL insertionAVL(pAVL a,pUsine u,int* h){
    if (a==NULL) {
        *h = 1;
        return creerAVL(u);
    }
    
    if (strcmp(u->id,a->usine.id) < 0){
        a->fg = insertionAVL(a->fg,u,h);
        *h = -*h;
    }else if(strcmp(u->id,a->usine.id) > 0){
        a->fd = insertionAVL(a->fd,u,h);
    }else{
        *h = 0;
        return a; 
    }

    if(*h != 0){
        a->eq += *h;
        if(a->eq==0){
            *h = 0;
        }else{
            *h = 1;
        }
    }

    return equilibrerAVL(a);
}


pAVL rechercheAVL(pAVL a,char* id){
    if (a==NULL){
        return NULL;
    }
    if(strcmp(id, a->usine.id)==0){
        return a;
    }else if(strcmp(id, a->usine.id) < 0){
        return rechercheAVL(a->fg,id);
    }else{
        return rechercheAVL(a->fd,id);
    }
}

void libererAVL(pAVL a){
    if(a ==NULL){
        return;
    }
    libererAVL(a->fg);
    libererAVL(a->fd);
    free(a->usine.id);
    free(a);
}

