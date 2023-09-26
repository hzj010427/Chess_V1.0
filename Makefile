#######################################################################
# Chess Program, for team10 of EECS 22L 
#
# Makefile: Makefile for Chess Program
#
#######################################################################

# Variables
CC	= gcc
CFLAGS	= -Wall -std=c11 -g

# Default target
all: bin/chess

######################### Generate object files #######################

# Target for chess_functions.o
src/chess_functions.o: src/chess_functions.c src/chess_functions.h src/constants.h src/board.h src/menu.h src/game.h
	$(CC) $(CFLAGS) -c src/chess_functions.c -o src/chess_functions.o

# Target for main.o
src/main.o: src/main.c src/board.h src/menu.h src/chess_functions.h src/constants.h src/game.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

# Target for board.o
src/board.o: src/board.c src/board.h src/constants.h
	$(CC) $(CFLAGS) -c src/board.c -o src/board.o

# Target for menu.o
src/menu.o: src/menu.c src/menu.h
	$(CC) $(CFLAGS) -c src/menu.c -o src/menu.o

# Target for game.o
src/game.o: src/game.c src/game.h src/chess_functions.h src/constants.h src/board.h src/menu.h
	$(CC) $(CFLAGS) -c src/game.c -o src/game.o

# Target for AI.o  
src/AI.o: src/AI.c src/AI.h src/chess_functions.h src/constants.h src/board.h
	$(CC) $(CFLAGS) -c src/AI.c -o src/AI.o

######################### Generate the executable #####################

# Create bin directory
bin:
	mkdir -p $@

# Target for chess
bin/chess: bin src/main.o src/chess_functions.o src/board.o src/menu.o src/game.o src/AI.o
	$(CC) $(CFLAGS) src/main.o src/chess_functions.o src/board.o src/menu.o src/game.o src/AI.o -o bin/chess

###############################  others  ##############################

# Target for run
run: bin/chess
	./bin/chess

# Target for clean-up

# Linux
clean:
	rm -f src/*.o bin/chess

# Windows
#clean:
#	del src\*.o
#	del bin\chess.exe

# Target for test

# Linux
test: bin/chess
	bin/chess < src/test_input.txt > src/test_output.txt
	echo "Test completed. Check scr/test_output.txt for results."

# Windows
#test: bin/chess
#	bin\chess < src\test_input.txt > src\test_output.txt
#	echo "Test completed. Check scr/test_output.txt for results."

# Target for tar
tar:
	tar czvf src/chess_source_code.tar.gz src/*.c src/*.h
	echo "Chess source code tarball created in the src directory."


