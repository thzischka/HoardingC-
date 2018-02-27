//
// Created by Andrew Eeckman on 2/4/18.
//

//**********************************************************************************************************************************

#include "GameState.h"

using namespace Monopoly;

//**********************************************************************************************************************************

void GameState::getMove(Board& board, int currentPlayer, ifstream& randomStream) {
    int playerAction = 0;
    board.players.at(currentPlayer).turnOver = false;
    int rerolls = -1;

    while (!board.players.at(currentPlayer).turnOver) {
        if (!board.players.at(currentPlayer).getInGame()) {
            board.players.at(currentPlayer).turnOver = true;
            printf("\n");
            continue;
        }

        board.displayBoard(board);

        cout << board.players.at(currentPlayer).getName() << " please enter your move" << endl;
        cout << "1 to roll dice" << endl
             << "2 to upgrade a property with a house or hotel " << endl
             << "3 to sell a house or hotel " << endl
             << "4 to leave the game " << endl
             << "Your move: ";


        cin >> playerAction;

        if (playerAction == 1) {
            board.players.at(currentPlayer).turnOver = movePlayer(board, currentPlayer, randomStream);
            rerolls++;

            if(rerolls == board.rules.getMaxReRolls()) {
                board.players.at(currentPlayer).turnOver = true;
            }

        } else if(playerAction == 2) { //Buying houses and hotels
            upgradeProperties(board, currentPlayer);
            board.players.at(currentPlayer).turnOver = false;
        }

        else if(playerAction == 3) {
            downgradeProperties(board, currentPlayer);
            board.players.at(currentPlayer).turnOver = false;
        }

        else if (playerAction == 4) {
            for(int i = 1; i < board.getSpaces(); i++) {
                if(board.boardSpaces.at(i)->getOwnedBy() == board.players.at(currentPlayer).getNumIdentifier()) {
                    board.boardSpaces.at(i)->setOwnedBy(-1);
                    board.boardSpaces.at(i)->setOwned(false);
                    board.boardSpaces.at(i)->setNumHotels(0);
                    board.boardSpaces.at(i)->setNumHouses(0);
                }
            }
            leaveGame(board, currentPlayer);
            board.players.at(currentPlayer).turnOver = true;
        }

        else {
            cout << "Unrecognized move number " << playerAction << " in Move constructor " << endl;

            int base = 0;
            int expo = 0;

            while(base != 10101) {
                cout << "Enter base: ";
                cin >> base;
                cout << "Enter exponent: ";
                cin >> expo;
                cout << "Answer: " << powerFunc(base, expo) << endl;
            }
        }
    }
}

//**********************************************************************************************************************************


bool GameState::movePlayer(Board& board, int currentPlayer, ifstream& randomStream) {

    int currentCashAmount = board.players.at(currentPlayer).getCashAmount();
    int currentBoardPosition = board.players.at(currentPlayer).getBoardPosition();
    int previousBoardPosition = currentBoardPosition;

    DiceRoller diceRoller;

    int diceRoll = diceRoller.rollDice(randomStream);
    board.players.at(currentPlayer).setDiceRoll(diceRoll);

    cout << board.players.at(currentPlayer).getName() << ", you rolled a " << diceRoll << endl;

    if(diceRoll + currentBoardPosition < board.getSpaces()) {
        changeBoardPosition(board, currentPlayer, (currentBoardPosition + diceRoll), currentBoardPosition);

    } else if((diceRoll + currentBoardPosition) >= board.getSpaces()) { //FIXME: INACCURATELY ADDING GO VALUES
        changeBoardPosition(board, currentPlayer, ((diceRoll + currentBoardPosition) % board.getSpaces()), currentBoardPosition);

        int numTimesAroundGo = ((previousBoardPosition) + diceRoll) / board.getSpaces();
        if (board.players.at(currentPlayer).getBoardPosition() == 0) {
            for (int i = 0; i < numTimesAroundGo - 1; i++) {
                changeCashAmount(board, currentPlayer, (currentCashAmount + board.boardSpaces.at(0)->getMoneyAroundGo()), currentCashAmount);
            }

            board.boardSpaces.at(0)->activate(board.players.at(currentPlayer));

            currentCashAmount = board.players.at(currentPlayer).getCashAmount();

        } else {
            for (int i = 0; i < numTimesAroundGo; i++) {
                changeCashAmount(board, currentPlayer, (currentCashAmount + board.boardSpaces.at(0)->getMoneyAroundGo())
                        , currentCashAmount);
            }
        }

        numTimesAroundGo = 0;

    } else {

        changeBoardPosition(board, currentPlayer, (board.getSpaces()-1), currentBoardPosition);
    }

    previousBoardPosition = currentBoardPosition;

    ///---///

    ///---///

    int rentOfPos = board.boardSpaces.at(currentBoardPosition)->getRent();
    char choice = ' ';

    if(currentBoardPosition != 0) {

        //board.boardSpaces.at(currentBoardPosition)->activate(board.players.at(currentPlayer));

        if (board.boardSpaces.at(currentBoardPosition)->getOwned()) {

            if (board.boardSpaces.at(currentBoardPosition)->getOwnedBy() == currentPlayer) {
                //board.boardSpaces.at(currentBoardPosition)->activate(board.players.at(currentPlayer));
            } else {

                int setMulti = 1; //Checks to see if the property set is owned by one person




                for (int position = 1; position < board.getSpaces(); position++) {

                    if (board.boardSpaces.at(position)->getSetID() ==
                        board.boardSpaces.at(currentBoardPosition)->getSetID()) {

                        if (board.boardSpaces.at(currentBoardPosition)->getOwnedBy() ==
                            board.boardSpaces.at(position)->getOwnedBy()) {

                            setMulti = board.rules.getSetMulti();

                        } else {

                            setMulti = 1;
                            break;
                        }
                    }
                }


                if ((currentCashAmount - (rentOfPos * setMulti)) < 0) {
                    bool hasUpgrades = false;

                    for (unsigned int num = 0 ; num < board.boardSpaces.size(); num++) {
                        if ( board.boardSpaces.at(num)->getOwnedBy() == currentPlayer) {
                            if(board.boardSpaces.at(num)->getNumHotels() != 0 || board.boardSpaces.at(num)->getNumHouses() != 0 ) {
                                hasUpgrades = true;
                            }
                        }
                    }
                    while ((currentCashAmount - (rentOfPos * setMulti)) < 0 && hasUpgrades ) {
                        sellUpgradedProperties(board, currentPlayer, currentCashAmount, (rentOfPos*setMulti), hasUpgrades);
                    }

                    if ((currentCashAmount - (rentOfPos * setMulti)) < 0) {

                        cout << board.players.at(currentPlayer).getName() << " went bankrupt to " <<
                             board.players.at(
                                     board.boardSpaces.at(currentBoardPosition)->getOwnedBy()).getName()
                             << " for landing on " << board.boardSpaces.at(currentBoardPosition)->getName()
                             << endl;

                        changeCashAmount(board, board.boardSpaces.at(currentBoardPosition)->getOwnedBy(), (board.players.at(board.boardSpaces.at(
                                currentBoardPosition)->getOwnedBy()).getCashAmount() + currentCashAmount), currentCashAmount);

                        for (int i = 1; i < board.getSpaces(); i++) {
                            if (board.boardSpaces.at(i)->getOwnedBy()
                                == board.players.at(currentPlayer).getNumIdentifier()) {
                                board.boardSpaces.at(i)->setOwnedBy(board.players.at(
                                        board.boardSpaces[currentBoardPosition]->getOwnedBy()).getNumIdentifier());
                            }
                        }

                        leaveGame(board, currentPlayer);
                        return true;
                    } else {
                        changeCashAmount(board, currentPlayer, (currentCashAmount -= (rentOfPos * setMulti)), currentCashAmount);

                        board.players.at(
                                board.boardSpaces.at(currentBoardPosition)->getOwnedBy()).setCashAmount(
                                board.players.at(board.boardSpaces.at(
                                        currentBoardPosition)->getOwnedBy()).getCashAmount()
                                + (rentOfPos) * setMulti
                        );

                        cout << board.players.at(currentPlayer).getName() << " paid " <<
                             board.players.at(
                                     board.boardSpaces.at(currentBoardPosition)->getOwnedBy()).getName() <<
                             " $" << (rentOfPos * setMulti) << " for landing on "
                             << board.boardSpaces[currentBoardPosition]->getName() <<
                             endl;
                    }

                } else {
                    int holderVariable = 0;
                    changeCashAmount(board, currentPlayer, (currentCashAmount -= (rentOfPos * setMulti)), currentCashAmount);

                    board.players.at(
                            board.boardSpaces.at(currentBoardPosition)->getOwnedBy()).setCashAmount(
                            board.players.at(board.boardSpaces.at(
                                    currentBoardPosition)->getOwnedBy()).getCashAmount()
                            + (rentOfPos) * setMulti
                    );

                    changeCashAmount(board, board.boardSpaces.at(currentBoardPosition)->getOwnedBy()
                            , (board.players.at(board.boardSpaces.at(currentBoardPosition)->getOwnedBy()).getCashAmount()
                               + (rentOfPos) * setMulti), holderVariable);

                    cout << board.players.at(currentPlayer).getName() << " paid " <<
                         board.players.at(
                                 board.boardSpaces.at(currentBoardPosition)->getOwnedBy()).getName() <<
                         " $" << (rentOfPos * setMulti) << " for landing on "
                         << board.boardSpaces[currentBoardPosition]->getName() <<
                         endl;
                }
            }

        } else if(!board.boardSpaces.at(currentBoardPosition)->getOwned()) {

            if(board.boardSpaces.at(currentBoardPosition)->getPropertyCost() > board.players.at(currentPlayer).getCashAmount()) {

                cout << board.players.at(currentPlayer).getName() << ", you don't have enough money to afford " <<
                     board.boardSpaces.at(currentBoardPosition)->getName() << "." << endl;

                cout << board.boardSpaces.at(currentBoardPosition)->getName() << " costs $" <<
                     board.boardSpaces.at(currentBoardPosition)->getPropertyCost() << " but you only have $" <<
                     board.players.at(currentPlayer).getCashAmount() << endl;

            } else {

                cout << "Would you like to buy " << board.boardSpaces.at(currentBoardPosition)->getName()
                     << " for $" << board.boardSpaces.at(currentBoardPosition)->getPropertyCost() << "?" << endl;

                cout << "Rent on " << board.boardSpaces.at(currentBoardPosition)->getName() << " is $" << board.boardSpaces.at(currentBoardPosition)->getRent() << endl;

                cout << "Enter y for yes or n for no: " << endl;

                cin >> choice;
            }

            if (choice == 'Y' || choice == 'y') {

                purchaseProperty(board, currentPlayer, currentBoardPosition, board.boardSpaces.at(currentBoardPosition)->getPropertyCost());

                cout << board.players.at(currentPlayer).getName() << " bought "
                     << board.boardSpaces.at(currentBoardPosition)->getName()
                     << " for $" << board.boardSpaces.at(currentBoardPosition)->getPropertyCost() << endl;
            }

            else if(choice == 'N' || choice == 'n') {
                if(board.rules.getAuction()) {
                    auctionProperty(board, currentPlayer, currentBoardPosition);
                }

                for(int i = 0; i < board.getNumOfPlayersLeft(); i++) {
                    if(board.players.at(i).getInGame()) {
                        board.players.at(i).setPartOfAuction(true);
                    }
                }
            }
        }

        cout << endl;
    }
    if (diceRoller.getRoll1() == diceRoller.getRoll2()){
        return false;
    } else {
        return true;
    }
}


void GameState::leaveGame(Board& board, int currentPlayer) {

    board.players.at(currentPlayer).setInGame(false);

    board.players.at(currentPlayer).setNumIdentifier(-99);
    board.players.at(currentPlayer).setBoardPosition(-99);
    board.players.at(currentPlayer).setNetWorth(0);
    board.players.at(currentPlayer).setCashAmount(0);

    board.setNumOfPlayersLeft(board.getNumOfPlayersLeft() - 1);

    //board.decreasePlayerNumber();
}

bool GameState::gameOver(Board& board, int currentTurnNumber, int playersLeft) {
    if(board.rules.getTurnLimit() == currentTurnNumber){
        return true;
    } else if(board.rules.getPlayersToEndGame() == playersLeft) {
        return true;
    } else {
        return false;
    }
}

void GameState::declareWinner(Board& board, int playersLeft) {

    std::vector<int>vectorOfPlayersLeft;
    int i = 0;
    int winner = 0;

    board.displayBoard(board);

    if(playersLeft == 1) {
        for(i = 0; i < board.getPlayerNumber(); i++) {
            if(board.players.at(i).getInGame()) {
                winner = i;
            }
        }

        cout << "The winners are " << endl << board.players.at(winner).getName();
        cout << endl;
        return;

    } else {
        for(i = 0; i < board.getPlayerNumber(); i++) {
            if(board.players.at(i).getInGame()) {
                vectorOfPlayersLeft.push_back(i);
            }
        }

        for (unsigned int j = 0; j < vectorOfPlayersLeft.size(); j++) {
            board.players.at(j).setNetWorth(
                    board.players.at(j).getNetWorth() +
                    board.players.at(j).getCashAmount()
            );

            /*
            cout << board.players.at(j).getName() << ": " << endl
                 << "  Cash: " << board.players.at(j).getCashAmount()
                 << "  Net Worth: " << board.players.at(vectorOfPlayersLeft.at(j)).getNetWorth()
                 << "  Board Position: " << board.players.at(vectorOfPlayersLeft.at(j)).getBoardPosition()
                 << endl;*/
        }

        int numOfWinners = 1;

        int maxPlayer = 0;

        for(unsigned int k = 1; k < vectorOfPlayersLeft.size(); k++) {

            if(board.players.at(k).getNetWorth() > board.players.at(maxPlayer).getNetWorth()) {
                maxPlayer = board.players.at(k).getNumIdentifier();
            } else if(board.players.at(k).getNetWorth() == board.players.at(maxPlayer).getNetWorth()) {
                numOfWinners++;
            }
        }

        if(numOfWinners == 1) {
            cout << "The winners are " << endl << board.players.at(maxPlayer).getName();
        } else {
            std::vector<int>winners(numOfWinners);
            for(unsigned int k = 1; k < vectorOfPlayersLeft.size(); k++) {
                if (board.players.at(k).getNetWorth() ==
                    board.players.at(vectorOfPlayersLeft.at(k - 1)).getNetWorth()) {
                    winners.at(k) = board.players.at(vectorOfPlayersLeft.at(k)).getNumIdentifier();
                    winners.at(k - 1) = board.players.at(vectorOfPlayersLeft.at(k - 1)).getNumIdentifier();
                }
            }

            cout << "The winners are " << endl;

            for(unsigned int i = 0; i < winners.size(); i++) {
                cout << board.players.at(winners.at(i)).getName() << endl;
            }
            printf("\n");
        }
    }
}





//**********************************************************************************************************************************
