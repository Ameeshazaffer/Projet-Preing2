CC = gcc

EXEC = cwildwater

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: $(EXEC)

%.o: %.c
	$(CC) -c $< -o $@

$(EXEC): $(OBJ)
	$(CC) $^ -o $@

clean:
	rm -f $(OBJ) $(EXEC)

