#!/bin/bash


START_TIME=$(date +%s%3N) # chrono

if [ $# -lt 2 ]; then
    echo "Erreur, pas assez d'arguments"
    echo "Voici comment faire : $0 <fichier> <commande> [options]"
    

    # Afficher la durée même en cas d'erreur
    END_TIME=$(date +%s%3N)
    DURATION=$((END_TIME - START_TIME))
    echo "Durée: $DURATION ms"
    exit 1
fi 


DATA_FILE="$1"
COMMAND="$2"


# est-ce que le fichier existe
if [ ! -f "$DATA_FILE" ]; then
    echo "Erreur, le fichier '$DATA_FILE' n'existe pas"
    END_TIME=$(date +%s%3N)
    DURATION=$((END_TIME - START_TIME))
    echo "Durée: $DURATION ms"
    exit 1
fi

EXEC_NAME="cwildwater"  # nom de l'exécutable C


# est-ce que l'exécutable existe, si non on compile
if [ ! -f "$EXEC_NAME" ]; then
    make
fi

case "$COMMAND" in

    histo)
        echo "Commande: histogramme"
        
        # vérifier qu'il y a un 3ème argument
        if [ $# -lt 3 ]; then
            echo "Erreur, il faut préciser le type d'histogramme"
            echo "Comment faire : $0 $DATA_FILE histo <max|src|real>"
            END_TIME=$(date +%s%3N)
            DURATION=$((END_TIME - START_TIME))
            echo "Durée: $DURATION ms"
            exit 1
        fi
        
        HISTO_TYPE="$3"  # max, src, real ou all
        
        # vérifier que le type est valide
        if [ "$HISTO_TYPE" != "max" ] && [ "$HISTO_TYPE" != "src" ] && [ "$HISTO_TYPE" != "real" ] && [ "$HISTO_TYPE" != "all" ]; then
            echo "Erreur: type invalide '$HISTO_TYPE'"
            echo "Types acceptés: max, src, real, all"
            END_TIME=$(date +%s%3N)
            DURATION=$((END_TIME - START_TIME))
            echo "Durée: $DURATION ms"
            exit 1
        fi
        
        # Vérifier qu'il n'y a pas d'arguments en trop
        if [ $# -gt 3 ]; then
            echo "Erreur: trop d'arguments"
            END_TIME=$(date +%s%3N)
            DURATION=$((END_TIME - START_TIME))
            echo "Durée: $DURATION ms"
            exit 1
        fi

        echo "Type d'histogramme: $HISTO_TYPE"
        
        # choisir le nom du fichier de sortie selon le choix de la personne
        if [ "$HISTO_TYPE" = "max" ]; then
            OUTPUT_FILE="capacite_max.dat"
        elif [ "$HISTO_TYPE" = "src" ]; then
            OUTPUT_FILE="vol_total_capte.dat"
        elif [ "$HISTO_TYPE" = "real" ]; then
            OUTPUT_FILE="vol_traitement.dat"
        else
            OUTPUT_FILE="vol_all.dat"
        fi
        
        echo "Traitement en cours..." # on va executer 
	    ./"$EXEC_NAME" "$DATA_FILE" histo "$HISTO_TYPE" "$OUTPUT_FILE"
	
	     # est-ce que le programme C s'est bien executé
	    if [ $? -ne 0 ]; then
            echo "Erreur: le programme C a échoué"
            END_TIME=$(date +%s%3N)
            DURATION=$((END_TIME - START_TIME))
            echo "Durée: $DURATION ms"
            exit 1
        fi

	
	    NB_LINES=$(($(wc -l < "$OUTPUT_FILE") - 1))
        
       
        # Graphique des 10 plus grandes usines
        if [ "$NB_LINES" -ge 10 ]; then
            GRAPH_TOP10="${OUTPUT_FILE%.dat}_top10.png"


