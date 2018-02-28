//
// Created by Andrew Eeckman on 2/4/18.
//

#ifndef HOARDINGCPP_GAMESTATE_H
#define HOARDINGCPP_GAMESTATE_H

//**********************************************************************************************************************************

#include "Board.h"
#include "DiceRoller.h"

//**********************************************************************************************************************************

namespace Monopoly {

    class GameState {
    public:
        void getMove(Board& board, int currentPlayer, ifstream& randomStream);
        bool movePlayer(Board& board, int currentPlayer, ifstream& randomStream);

        void changeBoardPosition(Board &board, int currentPlayer, int changeInPos, int& currentBoardPosition);
        void changeCashAmount(Board &board, int currentPlayer, int changeInVal, int& currentCashAmount);
        double powerFunc(double base, double expo = 0);

        void upgradeProperties(Board &board, int currentPlayer);
        void downgradeProperties(Board &board, int currentPlayer);

        void leaveGame(Board& board, int currentPlayer);
        bool gameOver(Board & board, int currentTurnNumber, int playersLeft);
        void declareWinner(Board& board, int playersLeft);
    };
}

//**********************************************************************************************************************************

#endif //HOARDINGCPP_GAMESTATE_H
