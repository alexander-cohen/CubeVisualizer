CC = g++
CARGS = -std=c++11 -g
INCLUDE = -I/Users/alexandercohen/LocalDocuments/OpenGL_Include
LIB_INCLUDE = -L/Users/alexandercohen/LocalDocuments/OpenGL_Lib
LIBS = -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi
FRAMEWORK = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

OBJ = obj/main.o obj/glad.o

cubeVisualizer: $(OBJ)
	$(CC) $(CARGS) $(INCLUDE) $(LIB_INCLUDE) $(LIBS) $(FRAMEWORK) $^ -o $@

obj/main.o: main.cpp
	$(CC) -c $(CARGS) $(INCLUDE) $^ -o $@

obj/glad.o: glad.c
	gcc -c $(INCLUDE) $^ -o $@

run: cubeVisualizer
	./cubeVisualizer

clean:
	-rm cubeVisualizer
	-rm obj/*
	-rm -r cubeVisualizer.dSYM

.PHONY: run clean