//
// Created by Andrew Eeckman on 2/25/18.
//
#include "Board.h"
#include "DiceRoller.h"

#ifndef HOARDINGCPP_GAMESTATELOGIC_H
#define HOARDINGCPP_GAMESTATELOGIC_H

namespace Monopoly {


    class GameStateLogic {
    public:
        void changeBoardPosition(Board &board, int currentPlayer, int changeInPos, int& currentBoardPosition);
        void changeCashAmount(Board &board, int currentPlayer, int changeInVal, int& currentCashAmount);

        void purchaseProperty(Board &board, int currentPlayer, int currentBoardPosition, int amountToPay);
        void auctionProperty(Board &board, int currentPlayer, int currentBoardPosition);

        void upgradeProperties(Board &board, int currentPlayer);
        void downgradeProperties(Board &board, int currentPlayer);
        void sellUpgradedProperties(Board &board, int currentPlayer, int& currentCashAmount, int rent, bool &hasUpgrades);

        double powerFunc(double base, double expo = 0);
    };
}

#endif //HOARDINGCPP_GAMESTATELOGIC_H
