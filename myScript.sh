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
        
        HISTO_TYPE="$3"  # max, src, real 
        
        # vérifier que le type est valide
        if [ "$HISTO_TYPE" != "max" ] && [ "$HISTO_TYPE" != "src" ] && [ "$HISTO_TYPE" != "real" ]; then
            echo "Erreur: type invalide '$HISTO_TYPE'"
            echo "Types acceptés: max, src, real"
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
        else 
            OUTPUT_FILE="vol_total_traite.dat"
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

	          # Créer un fichier de script pour les 10 plus grandes usines 
            echo "set terminal png size 1200,800" > gnuplot_top10.tmp
            echo "set output \"$GRAPH_TOP10\"" >> gnuplot_top10.tmp
            echo "set datafile separator \";\"" >> gnuplot_top10.tmp
            echo "set style data histograms" >> gnuplot_top10.tmp
            echo "set style fill solid" >> gnuplot_top10.tmp
            echo "set xtics rotate by -45" >> gnuplot_top10.tmp
            echo "set ylabel \"Volume (M.m3/an)\"" >> gnuplot_top10.tmp
            echo "set title \"Top 10 des plus grandes usines\"" >> gnuplot_top10.tmp
            echo "plot '$OUTPUT_FILE' every ::1::10 using 2:xtic(1) notitle" >> gnuplot_top10.tmp

	         # Lancer gnuplot avec le script
            gnuplot gnuplot_top10.tmp
            
            if [ -f "$GRAPH_TOP10" ]; then
                echo "Graphique créé: $GRAPH_TOP10"
            fi
	    fi


 	      # Graphique des 50 plus petites usines
        if [ "$NB_LINES" -ge 50 ]; then
            GRAPH_BOTTOM50="${OUTPUT_FILE%.dat}_bottom50.png"
            
            # calculer à partir de quelle ligne commencer
            START_LINE=$((NB_LINES - 49))
            
            echo "set terminal png size 1600,800" > gnuplot_bottom50.tmp
            echo "set output \"$GRAPH_BOTTOM50\"" >> gnuplot_bottom50.tmp
            echo "set datafile separator \";\"" >> gnuplot_bottom50.tmp
            echo "set style data histograms" >> gnuplot_bottom50.tmp
            echo "set style fill solid" >> gnuplot_bottom50.tmp
            echo "set xtics rotate by -90 font \",8\"" >> gnuplot_bottom50.tmp
            echo "set ylabel \"Volume (M.m3/an)\"" >> gnuplot_bottom50.tmp
            echo "set title \"50 plus petites usines\"" >> gnuplot_bottom50.tmp
            echo "plot '$OUTPUT_FILE' every ::$START_LINE::$NB_LINES using 2:xtic(1) notitle" >> gnuplot_bottom50.tmp
            
            gnuplot gnuplot_bottom50.tmp
            
            if [ -f "$GRAPH_BOTTOM50" ]; then
                echo "Graphique créé: $GRAPH_BOTTOM50"
            fi
	     fi
	;;


    leaks)
	      echo "Commande: calcul des pertes"
	      if [ $# -lt 3 ]; then
            echo "Erreur: il faut préciser l'identifiant de l'usine"
            echo "Usage: $0 $DATA_FILE leaks \"<identifiant>\""
            END_TIME=$(date +%s%3N)
            DURATION=$((END_TIME - START_TIME))
            echo "Durée: $DURATION ms"
            exit 1
        fi
        
        FACILITY_ID="$3"  # L'identifiant de l'usine
        
        # Vérifier qu'il n'y a pas d'arguments en trop
        if [ $# -gt 3 ]; then
            echo "Erreur: trop d'arguments"
            END_TIME=$(date +%s%3N)
            DURATION=$((END_TIME - START_TIME))
            echo "Durée: $DURATION ms"
            exit 1
        fi

	    echo "Usine: $FACILITY_ID"
        
        OUTPUT_FILE="leaks.dat"
        
        # Appeler le programme C
        ./"$EXEC_NAME" "$DATA_FILE" leaks "$FACILITY_ID" "$OUTPUT_FILE"
        
        # Vérifier que ça a fonctionné
        if [ $? -ne 0 ]; then
            echo "Erreur: le programme C a échoué"
            END_TIME=$(date +%s%3N)
            DURATION=$((END_TIME - START_TIME))
            echo "Durée: $DURATION ms"
            exit 1
        fi
        
        if [ ! -f "$OUTPUT_FILE" ]; then
            echo "Erreur: le fichier de sortie n'a pas été créé"
            END_TIME=$(date +%s%3N)
            DURATION=$((END_TIME - START_TIME))
            echo "Durée: $DURATION ms"
            exit 1
        fi
        
        echo "Résultats enregistrés dans: $OUTPUT_FILE"
        
        # Afficher le dernier résultat
        # tail -n 1 = dernière ligne du fichier
        echo "Dernier résultat:"
        tail -n 1 "$OUTPUT_FILE"
        ;;

