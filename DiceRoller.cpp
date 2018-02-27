//
// Created by Andrew Eeckman on 2/7/18.
//

#include "DiceRoller.h"

using namespace Monopoly;

DiceRoller::DiceRoller() {
    diceRoll = 0;
}

int DiceRoller::rollDice(ifstream &randomStream) {
    diceRoll = 0;
    roll1 = 0;
    roll2 = 0;

    randomStream >> roll1;
    randomStream >> roll2;

    roll1 = (roll1 % 6) + 1;
    roll2 = (roll2 % 6) + 1;
    diceRoll = roll1 + roll2;

    return diceRoll;
}

int DiceRoller::getRoll1(){
    return (roll1);
}
int DiceRoller::getRoll2(){
    return (roll2);
}
