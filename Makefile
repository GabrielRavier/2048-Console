default: 2048
	rm *.o

2048: main.o Game.o
	g++ -o 2048 main.o Game.o

main.o: main.cpp
	g++ -c main.cpp

Game.o: Game.cpp
	g++ -c Game.cpp

clean:
	rm *.o 2048
