CC = g++
FLAGS = -std=c++20 -Werror -Wall -Wextra -pedantic

all: main.o
	$(CC) $(FLAGS) main.o -o solution

main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp

clear:
	rm -f *.o solution