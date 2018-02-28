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

            board.boardSpaces.at(currentBoardPosition)->activate(board.players.at(currentPlayer));

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

    ///---///

    ///---///


    if(currentBoardPosition != 0) {
        board.boardSpaces.at(currentBoardPosition)->activate(board.players.at(currentPlayer));
        if(!board.players.at(currentPlayer).getInGame()) {
            leaveGame(board, currentPlayer);
            return true;
        }
    }

    if (diceRoller.getRoll1() == diceRoller.getRoll2()){
        return false;
    } else {
        return true;
    }
}


void GameState::upgradeProperties(Board &board, int currentPlayer) {
    std::vector <int> upgradable;
    bool canUpgrade = true;
    for (unsigned int i = 1; i <  board.boardSpaces.size(); i++) { //Cycles throught the properties looking for the upgradeable ones

        canUpgrade = true; //reset the value before execution of loop
        if (board.boardSpaces.at(i)->getNumHotels() == 1) {
            canUpgrade = false;
            continue;
        }
        if(board.boardSpaces.at(i)->getNumHouses() < board.rules.getHousesBefore()){
            if (board.boardSpaces.at(i)->getHouseCost() >= board.players.at(currentPlayer).getCashAmount()) {
                canUpgrade = false;
                continue;  //Checks for enough cash
            }
        } else if ( board.boardSpaces.at(i)->getNumHouses() == board.rules.getHousesBefore()) {
            if (board.boardSpaces.at(i)->getHotelCost() >= board.players.at(currentPlayer).getCashAmount()) {
                canUpgrade = false;
                continue;  //Checks for enough cash
            }
        }
        if (board.boardSpaces.at(i)->getOwnedBy() == currentPlayer) {
            for(unsigned int j = 0; j < board.boardSpaces.size(); j++){
                if(board.boardSpaces.at(i)->getSetID() == board.boardSpaces.at(j)->getSetID()){
                    if(board.boardSpaces.at(i)->getOwnedBy() != board.boardSpaces.at(j)->getOwnedBy()) {
                        if(board.boardSpaces.at(i)->getName() == "L.A.") {
                            continue;
                        }
                        canUpgrade = false;
                        break; //checks for owning all properties
                    }
                    if(board.rules.getBuildEvenly()) {
                        if(board.boardSpaces.at(j)->getNumHotels() == 1) {
                            //Just to make sure that it can catch if a propety has a hotel and no houses
                        } else if (board.boardSpaces.at(i)->getNumHouses() > board.boardSpaces.at(j)->getNumHouses()) {
                            canUpgrade = false;
                            break; //checks for even building
                        }
                    }
                }
            }

            bool foundSet = false;

            if(canUpgrade) {
                if(board.boardSpaces.at(i)->getIntraID() == 0) {
                    upgradable.push_back(i);
                }
                else if (upgradable.empty()) {
                    upgradable.push_back(i);
                } else if (!upgradable.empty()) {
                    for (unsigned int pos = 0; pos < upgradable.size(); pos++) {
                        if (foundSet && (board.boardSpaces.at(i)->getSetID() <
                                         board.boardSpaces.at(upgradable.at(pos))->getSetID())) {
                            upgradable.insert(upgradable.begin() + (pos),
                                              i); //puts element at end of sets if they are middle sets
                            break;
                        } else if (board.boardSpaces.at(i)->getSetID() <
                                   board.boardSpaces.at(upgradable.at(pos))->getSetID()) {
                            //put at beginning
                            upgradable.insert(upgradable.begin(), i);
                            break;
                        } else if (board.boardSpaces.at(i)->getSetID() ==
                                   board.boardSpaces.at(upgradable.at(pos))->getSetID()) {
                            foundSet = true; //enables the elements to be place at the end of middle sets
                            if ((pos + 1) == upgradable.size()){
                                upgradable.push_back(i);
                                break;
                            }
                        } else if (board.boardSpaces.at(i)->getSetID() >
                                   board.boardSpaces.at(upgradable.at(pos))->getSetID()) {
                            if (board.boardSpaces.at(i)->getSetID() == board.boardSpaces.at(upgradable.back())->getSetID()) {
                                //put at end
                                upgradable.push_back(i);
                                break;
                            } else {
                                if ((pos + 1) == upgradable.size()){
                                    upgradable.push_back(i);
                                    break;
                                }
                                continue;
                            }
                        }

                    }
                }

            }

        }

    }
    if(upgradable.size() == 0){
        cout << "You don't have any properties that you can upgrade" << endl;
    } else {
        cout << "Which property do you want to upgrade?" << endl;
        for ( unsigned int i = 0; i < upgradable.size(); i++) {
            if (board.boardSpaces.at(upgradable.at(i))->getNumHouses() < board.rules.getHousesBefore()) {
                cout << i << ". " << board.boardSpaces.at(upgradable.at(i))->getName() << " [$"
                     << board.boardSpaces.at(upgradable.at(i))->getHouseCost() << "]" << std::endl;
            }
            if (board.boardSpaces.at(upgradable.at(i))->getNumHouses() == board.rules.getHousesBefore()) {
                cout << i << ". " << board.boardSpaces.at(upgradable.at(i))->getName() << " [$"
                     << board.boardSpaces.at(upgradable.at(i))->getHotelCost() << "]" << std::endl;
            }
        }
        int userChoice;
        cout << "Your choice: ";
        std::cin >> userChoice;
        if (board.boardSpaces.at(upgradable.at(userChoice))->getNumHouses() < board.rules.getHousesBefore()) {

            board.boardSpaces.at(upgradable.at(userChoice))->setNumHouses(board.boardSpaces.at(upgradable.at(userChoice))->getNumHouses() + 1);
            board.players.at(currentPlayer).setCashAmount((board.players.at(currentPlayer).getCashAmount() - board.boardSpaces.at(upgradable.at(userChoice))->getHouseCost()));
            board.boardSpaces.at(upgradable.at(userChoice))->setRent(board.boardSpaces.at(upgradable.at(userChoice))->getRentHouse() * (int)powerFunc(2, (board.boardSpaces.at(upgradable.at(userChoice))->getNumHouses() - 1)));

        } else if (board.boardSpaces.at(upgradable.at(userChoice))->getNumHouses() == board.rules.getHousesBefore()) {
            board.boardSpaces.at(upgradable.at(userChoice))->setNumHouses(0);
            board.boardSpaces.at(upgradable.at(userChoice))->setNumHotels(1);
            board.players.at(currentPlayer).setCashAmount(board.players.at(currentPlayer).getCashAmount() - board.boardSpaces.at(upgradable.at(userChoice))->getHotelCost());
            board.boardSpaces.at(upgradable.at(userChoice))->setRent(board.boardSpaces.at(upgradable.at(userChoice))->getRentHotel());
        }
    }

    //Print out the choice and get the choice and change info

}

void GameState::downgradeProperties(Board &board, int currentPlayer) {
    int currentCashAmount = board.players.at(currentPlayer).getCashAmount();
    std::vector <int> degradable;
    bool canDegrade = false;
    for (unsigned int i = 0; i <  board.boardSpaces.size(); i++) { //Cycles throught the properties looking for the upgradeable ones
        canDegrade = true; //reset the value before execution of loop
        if (board.boardSpaces.at(i)->getNumHouses() == 0 && board.boardSpaces.at(i)->getNumHotels() != 1) {
            canDegrade = true; //Checks for a hotel on the property
            continue;
        }
        if (board.boardSpaces.at(i)->getOwnedBy() == currentPlayer) {
            for(unsigned int j = 0; j < board.boardSpaces.size(); j++){
                if(board.boardSpaces.at(i)->getSetID() == board.boardSpaces.at(j)->getSetID()){
                    if(board.rules.getBuildEvenly()) {
                        if(board.boardSpaces.at(i)->getNumHotels() == 1) {
                            //Just to make sure that it can catch if a propety has a hotel and no houses
                        } else if (board.boardSpaces.at(j)->getNumHotels() == 1) {
                            canDegrade = false;
                            break;
                        } else if (board.boardSpaces.at(i)->getNumHouses() < board.boardSpaces.at(j)->getNumHouses()) {
                            canDegrade = false;
                            break; //checks for even building
                        }
                    }
                }
            }

            bool foundSet = false;
            if(canDegrade) {
                if (degradable.empty() && i != 0) {
                    degradable.push_back(i);
                } else if (i >= 1 && !degradable.empty()) {
                    for (unsigned int pos = 0; pos < degradable.size(); pos++) {
                        if (foundSet && (board.boardSpaces.at(i)->getSetID() <
                                         board.boardSpaces.at(degradable.at(pos))->getSetID())) {
                            degradable.insert(degradable.begin() + (pos),
                                              i); //puts element at end of sets if they are middle sets
                            break;
                        } else if (board.boardSpaces.at(i)->getSetID() <
                                   board.boardSpaces.at(degradable.at(pos))->getSetID()) {
                            //put at beginning
                            degradable.insert(degradable.begin(), i);
                            break;
                        } else if (board.boardSpaces.at(i)->getSetID() ==
                                   board.boardSpaces.at(degradable.at(pos))->getSetID()) {
                            foundSet = true; //enables the elements to be place at the end of middle sets
                            if ((pos + 1) == degradable.size()){
                                degradable.push_back(i);
                                break;
                            }
                        } else if (board.boardSpaces.at(i)->getSetID() >
                                   board.boardSpaces.at(degradable.at(pos))->getSetID()) {
                            if (board.boardSpaces.at(i)->getSetID() == board.boardSpaces.at(degradable.back())->getSetID()) {
                                //put at end
                                degradable.push_back(i);
                                break;
                            } else {
                                if ((pos + 1) == degradable.size()){
                                    degradable.push_back(i);
                                    break;
                                }
                                continue;
                            }
                        }

                    }
                }
            }

        }

    }
    if(degradable.size() == 0){
        cout << "You don't have any upgrades that you can sell" << endl;
    } else {
        cout << "Which property do you want to downgrade?" << endl;
        for (unsigned int i = 0; i < degradable.size(); i++) {
            if (board.boardSpaces.at(degradable.at(i))->getNumHouses() > 0) {
                cout << i << ". " << board.boardSpaces.at(degradable.at(i))->getName() << " [$"
                     << (board.boardSpaces.at(degradable.at(i))->getHouseCost()/2) << "]" << std::endl;
            } else if (board.boardSpaces.at(degradable.at(i))->getNumHotels() == 1 ){
                cout << i << ". " << board.boardSpaces.at(degradable.at(i))->getName() << " [$"
                     << (board.boardSpaces.at(degradable.at(i))->getHotelCost()/2) << "]" << std::endl;
            }
        }
        int userChoice;
        cout << "Your choice: ";
        std::cin >> userChoice;
        if (board.boardSpaces.at(degradable.at(userChoice))->getNumHouses() > 0) {

            board.boardSpaces.at(degradable.at(userChoice))->setNumHouses(board.boardSpaces.at(degradable.at(userChoice))->getNumHouses() - 1);
            changeCashAmount(board, currentPlayer
                    , ((board.players.at(currentPlayer).getCashAmount() + (board.boardSpaces.at(degradable.at(userChoice))->getHouseCost()/2)))
                    , currentCashAmount);

            if (board.boardSpaces.at(degradable.at(userChoice))->getNumHouses() != 0) {

                board.boardSpaces.at(degradable.at(userChoice))->setRent(
                        board.boardSpaces.at(degradable.at(userChoice))->getRentHouse() *
                        (int) powerFunc(2, (board.boardSpaces.at(
                                degradable.at(userChoice))->getNumHouses() - 1)));

            } else if (board.boardSpaces.at(degradable.at(userChoice))->getNumHouses() == 0) {

                board.boardSpaces.at(degradable.at(userChoice))->setRent(board.boardSpaces.at(degradable.at(userChoice))->getStartRent());
            }

        } else if (board.boardSpaces.at(degradable.at(userChoice))->getNumHotels() == 1) {

            board.boardSpaces.at(degradable.at(userChoice))->setNumHouses(4);
            board.boardSpaces.at(degradable.at(userChoice))->setNumHotels(0);

            changeCashAmount(board, currentPlayer
                    , (board.players.at(currentPlayer).getCashAmount() + (board.boardSpaces.at(degradable.at(userChoice))->getHotelCost()/2))
                    , currentCashAmount);

            board.boardSpaces.at(degradable.at(userChoice))->setRent(board.boardSpaces.at(degradable.at(userChoice))->getRentHouse() * (int)powerFunc(2, (board.boardSpaces.at(degradable.at(userChoice))->getNumHouses() - 1)));

        }
    }
}

void GameState::changeBoardPosition(Board &board, int currentPlayer, int changeInPos, int& currentBoardPosition) {
    board.players.at(currentPlayer).setBoardPosition(changeInPos);
    currentBoardPosition = board.players.at(currentPlayer).getBoardPosition();
}

void GameState::changeCashAmount(Board &board, int currentPlayer, int changeInVal, int &currentCashAmount) {
    board.players.at(currentPlayer).setCashAmount(changeInVal);
    currentCashAmount = board.players.at(currentPlayer).getCashAmount();
}

double GameState::powerFunc(double base, double expo) {
    if(expo <= 0) {
        return 1;
    }

    return base * powerFunc(base, expo - 1);
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
