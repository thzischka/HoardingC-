hoarding.out: main.o Rules.o Board.o Player.o GameState.o DiceRoller.o Property.o Space.o
	g++ -g -Wall -Werror -o hoarding.out main.o Rules.o Board.o Player.o GameState.o DiceRoller.o Property.o Space.o

main.o: main.cpp Rules.h Board.h Player.h GameState.h DiceRoller.h
	g++ -g -Wall -Werror -c main.cpp

Rules.o: Rules.cpp Rules.h
	g++ -g -Wall -Werror -c Rules.cpp

Board.o: Board.cpp Board.h Property.h Player.h Space.h
	g++ -g -Wall -Werror -c Board.cpp

Player.o: Player.cpp Player.h Rules.h
	g++ -g -Wall -Werror -c Player.cpp

GameState.o: GameState.cpp GameState.h Board.h Rules.h Player.h Property.h Space.h DiceRoller.h
	g++ -g -Wall -Werror -c GameState.cpp

DiceRoller.o: DiceRoller.cpp DiceRoller.h
	g++ -g -Wall -Werror -c DiceRoller.cpp

Property.o: Property.cpp Property.h
	g++ -g -Wall -Werror -c Property.cpp

Space.o: Space.cpp Space.h
	g++ -g -Wall -Werror -c Space.cpp

clean:
	rm -fr *.o *.out