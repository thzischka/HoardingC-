//
// Created by Andrew Eeckman on 1/31/18.
//

#ifndef HOARDINGCPP_PLAYERMANAGER_H
#define HOARDINGCPP_PLAYERMANAGER_H

//**********************************************************************************************************************************

#include <string>
#include <iostream>
#include <vector>

#include "Rules.h"

//**********************************************************************************************************************************

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;

//**********************************************************************************************************************************

namespace Monopoly {

    class Player {
    public:

        Player();

        /***** BASE VARIABLES *****/

        void setName(const string &n) {
            name.assign(n);
        }
        void setNumIdentifier(int num) {
            numIdentifier = num;
        }
        void setCashAmount(int cash) {
            cashAmount = cash;
        }
        void setNetWorth(int worth) {
            netWorth = worth;
        }
        void setBoardPosition(int pos) {
            boardPosition = pos;
        }
        void setInGame(bool inG) {
            inGame = inG;
        }

        string getName() {
            return name;
        }
        int getNumIdentifier() {
            return numIdentifier;
        }
        int getCashAmount() {
            return cashAmount;
        }
        int getNetWorth() {
            return netWorth;
        }
        int getBoardPosition() {
            return boardPosition;
        }
        bool getInGame() {
            return inGame;
        }

        /***** SECONDARY VARIABLES *****/

        void setPartOfAuction(bool inAuction) {
            partOfAuction = inAuction;
        }

        bool getPartOfAuction() {
            return partOfAuction;
        }

        void setDiceRoll(int roll) {
            diceRoll = roll;
        }

        int getDiceRoll() {
            return diceRoll;
        }

        bool turnOver;

    private:
        /***** BASE VARIABLES *****/
        string name;
        int numIdentifier;
        int cashAmount;
        int netWorth;
        int boardPosition;
        bool inGame;

        /***** SECONDARY VARIABLES *****/
        bool partOfAuction;
        int diceRoll;
    };
}

//**********************************************************************************************************************************

#endif //HOARDINGCPP_PLAYERMANAGER_H
