CC = gcc
CFLAGS = -Wall -Wextra -O3 -std=c99
LIBS = -lraylib -lm -lpthread -ldl -lrt -lX11

SRC = main.c physics.c render.c init.c
OBJ = $(SRC:.c=.o)
EXEC = graviton

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

