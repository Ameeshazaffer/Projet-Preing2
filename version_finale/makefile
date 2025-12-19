CC = gcc
CFLAGS = -Wall -Wextra -g
EXEC = cwildwater
OBJETS = main.o histogramme.o leaks.o


$(EXEC): $(OBJETS)
	$(CC) $(OBJETS) -o $(EXEC)

main.o: main.c histogramme.h leaks.h
	$(CC) $(CFLAGS) -c main.c

histogramme.o: histogramme.c histogramme.h
	$(CC) $(CFLAGS) -c histogramme.c

leaks.o: leaks.c leaks.h
	$(CC) $(CFLAGS) -c leaks.c

clean:
	rm -f $(OBJETS)

fclean: clean
	rm -f $(EXEC)

