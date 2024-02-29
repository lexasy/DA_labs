CC = g++
FLAGS = -std=c++20 -Werror -Wall -Wextra -pedantic

all: main.o
	$(CC) $(FLAGS) main.o -o solution

lab1.o: lab1.cpp
	$(CC) $(FLAGS) -c main.cpp

clear:
	rm -f *.o solution