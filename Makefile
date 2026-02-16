CC = gcc
CFLAGS = -Wall -Wextra -O3 -std=c99 -fopenmp -ffast-math -march=native
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -fopenmp

SRC = main.c physics.c render.c init.c
OBJ = $(SRC:.c=.o)
EXEC = graviton

LAUNCHER_SRC = launcher.c
LAUNCHER_EXEC = launch


all: $(EXEC) $(LAUNCHER_EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LIBS)

$(LAUNCHER_EXEC): $(LAUNCHER_SRC)
		 $(CC) $(CFLAGS) $(LAUNCHER_SRC) -o $(LAUNCHER_EXEC) $(LIBS)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC) $(LAUNCHER_EXEC)

