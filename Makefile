FLAGS = -Wall -Wextra -pedantic-errors

INCLUDES = -I ./Source/ -I ./Include/
SOURCES =  ./Source/STD/*.cpp ./Tests/*.cpp

all:
	g++ $(SOURCES) -o ./Bin/Tests -std=c++17 $(INCLUDES) $(FLAGS)