CC = gcc
CFLAGS = -Wall -std=c99 -I"C:/raylib/raylib-5.0_win64_mingw-w64/include"
LIBS = -L"C:/raylib/raylib-5.0_win64_mingw-w64/lib" -lraylib -lopengl32 -lgdi32 -lwinmm
SRC = main.c memoria.c algoritmos.c ia.c
OBJ = $(SRC:.c=.o)
EXEC = jogo

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC) $(LIBS)

run: all
	./$(EXEC)

clean:
	del *.o
	del $(EXEC).exe
