CCFLAGS = -Wall
CC = g++

p : main.o
	$(CC) main.o -o p  -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network -lGL

main.o : main.cpp
	$(CC) $(CCFLAGS) -c main.cpp
.PHONY : clean
clean:
	rm -f p main.o core *~