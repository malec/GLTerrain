# define libraries
LIB = -lGL -lGLU -lglut

# define g++ flags
CC = g++

project3: project3.cpp
	$(CC) -o gouraud project3.cpp $(LIB)