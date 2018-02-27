#include <iostream>
#include <fstream>
#include <string>

#include "Rules.h"
#include "Board.h"
#include "Player.h"
#include "GameState.h"
#include "DiceRoller.h"

//**********************************************************************************************************************************

using namespace Monopoly;
using std::cout;
using std::cin;
using std::endl;

//**********************************************************************************************************************************

int main(int argc, char* argv[]) {
    Board board;
    GameState game;

    ifstream randomStream;

    int turnCounter = 0;

    //readInRules(game, argv[1]);

    board.readInRules(argv[1]);

    board.readInBoard(argv[2], board);

    board.initiatePlayers(board);

    randomStream.open(argv[3]);


    while(!game.gameOver(board, turnCounter, board.getNumOfPlayersLeft())) {
        game.getMove(board, turnCounter % board.getPlayerNumber(), randomStream);
        turnCounter++;
    }

    game.declareWinner(board, board.getNumOfPlayersLeft());

    randomStream.close();

    return 0;
}

//**********************************************************************************************************************************

