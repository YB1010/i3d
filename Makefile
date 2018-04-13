SOURCES=assignment1.c
OBJECTS=assignment1.o
PROGRAM=assignment1
FLAGS=-lGL -lGLU -lglut -lm


all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	gcc -o $(PROGRAM) $(OBJECTS) $(FLAGS)

assignment1.o: assignment1.c
	gcc -c assignment1.c $(FLAGS)

clean:
	rm $(PROGRAM): $(OBJECTS)
