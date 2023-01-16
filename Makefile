FLAGS = -Wall -Wextra -pedantic-errors

INCLUDES = -I ./Source/ -I ./Include/
SOURCES = ./Source/*.cpp ./Source/STD/*.cpp

all:
	g++ $(SOURCES) -o ./Bin/App -std=c++17 $(INCLUDES) $(FLAGS)