@gcc *.c include/*.c lib/*.c -o game.exe -I include -I lib -I C:/raylib/raylib/src -L C:/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm
@game.exe