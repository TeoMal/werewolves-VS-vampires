# source directory
SRC = ./src

# object files
OBJ = $(SRC)/main.o \
	  $(SRC)/gameplay.o \
	  $(SRC)/entities.o \
	  $(SRC)/gameLoop.o	\
	  $(SRC)/point.o \
	  $(SRC)/colors.o

# executable program name
EXEC = WnV

# compiler
CC = g++

# flags
flags = -g -Wall -Wextra -Wfatal-errors -fmax-errors=100 -Wshadow -lncurses 

# create the executable program
WnV: $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(flags)

main.o: main.cpp
	$(CC) -c main.cpp $(flags)

gameplay.o: gameplay.cpp
	$(CC) -c gameplay.cpp $(flags)

entities.o: entities.cpp
	$(CC) -c entities.cpp $(flags)

gameLoop.o: gameLoop.cpp
	$(CC) -c gameLoop.cpp $(flags)

point.o: point.cpp
	$(CC) -c point.cpp $(flags)

colors.o: colors.cpp
	$(CC) -c colors.cpp $(flags)

clear:
	rm -f $(OBJ) $(EXEC)
	clear

clean:
	rm -f $(OBJ) $(EXEC)
	clear

help: WnV
	valgrind --leak-check=full -v --show-leak-kinds=all --track-origins=yes ./$(EXEC) 15 15

run: WnV
	./WnV 20 20 vampires
