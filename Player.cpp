//
// Created by Andrew Eeckman on 1/31/18.
//

#include "Player.h"

using namespace Monopoly;

//**********************************************************************************************************************************

Player::Player() {
    name.assign(" ");
    numIdentifier = 0;
    cashAmount = 0;
    netWorth = 0;
    boardPosition = 0;
    inGame = true;

    partOfAuction = true;
    turnOver = false;
    diceRoll = 0;
}

//**********************************************************************************************************************************

