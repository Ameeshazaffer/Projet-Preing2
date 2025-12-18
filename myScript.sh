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
