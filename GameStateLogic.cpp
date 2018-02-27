//
// Created by Andrew Eeckman on 2/25/18.
//

#include "GameStateLogic.h"

using namespace Monopoly;

/***** Change Positioning Or Cash Value Amounts *****/








void GameStateLogic::changeBoardPosition(Board &board, int currentPlayer, int changeInPos, int& currentBoardPosition) {

    board.players.at(currentPlayer).setBoardPosition(changeInPos);

    currentBoardPosition = board.players.at(currentPlayer).getBoardPosition();

}

void GameStateLogic::changeCashAmount(Board &board, int currentPlayer, int changeInVal, int &currentCashAmount) {

    board.players.at(currentPlayer).setCashAmount(changeInVal);

    currentCashAmount = board.players.at(currentPlayer).getCashAmount();
}








/***** Purchase Or Auction Off Properties *****/








void GameStateLogic::purchaseProperty(Board &board, int currentPlayer, int currentBoardPosition, int amountToPay) {

    int currentCashAmount = board.players.at(currentPlayer).getCashAmount();

    changeCashAmount(board, currentPlayer, (currentCashAmount - amountToPay), currentCashAmount);

    board.boardSpaces.at(currentBoardPosition)->setOwned(true);

    board.boardSpaces.at(currentBoardPosition)->setOwnedBy(board.players.at(currentPlayer).getNumIdentifier());

    board.players.at(currentPlayer).setNetWorth(
            board.players.at(currentPlayer).getNetWorth() + board.boardSpaces.at(currentBoardPosition)->getPropertyCost()
    );
}

void GameStateLogic::auctionProperty(Board &board, int currentPlayer, int currentBoardPosition) {

    int highestBid = 0; // Money Value
    int playerBid = 0;
    int highestBidder = 0; // Num Identifier

    int playersInAuction = board.getNumOfPlayersLeft();

    bool auctionOver = false;

    cout << "Starting the auction for " << board.boardSpaces.at(currentBoardPosition)->getName() << endl;


    while(!auctionOver) {

        if(!board.players.at(currentPlayer).getPartOfAuction() || board.players.at(currentPlayer).getCashAmount() < highestBid) {
            currentPlayer = (currentPlayer + 1) % board.getNumOfPlayersLeft();
        }

        if(highestBid == 0) {
            cout << "No one has bid on " << board.boardSpaces.at(currentBoardPosition)->getName()
                 << " [$" << board.boardSpaces.at(currentBoardPosition)->getPropertyCost() << "] yet"
                 << endl;
        }


        cout << board.players.at(currentPlayer).getName() << ", enter a bid of at least $" << highestBid + 1
             << " to bid on the property or a value less than that to leave the auction " << endl
             << "Your bid: ";

        cin >> playerBid;

        if(playerBid > highestBid) {
            highestBid = playerBid;
            highestBidder = board.players.at(currentPlayer).getNumIdentifier();
            currentPlayer = (currentPlayer + 1) % board.getNumOfPlayersLeft();

        } else {
            board.players.at(currentPlayer).setPartOfAuction(false);
            playersInAuction -= 1;
            currentPlayer = (currentPlayer + 1) % board.getNumOfPlayersLeft();
        }

        if(playersInAuction == 1 && highestBid > 0) {
            auctionOver = true;
            break;
        } else if(playersInAuction == 0 && highestBid == 0) {
            auctionOver = true;
            break;
        }

        if(highestBid > 0) {
            cout << "The current bid for " << board.boardSpaces.at(currentBoardPosition)->getName()
                 << " [$" << board.boardSpaces.at(currentBoardPosition)->getPropertyCost() << "] "
                 << "is $" << highestBid << " by " << board.players.at(highestBidder).getName() << endl;
        }
    }


    if(highestBid > 0) {
        cout << board.players.at(highestBidder).getName() << " won "
             << board.boardSpaces.at(currentBoardPosition)->getName()
             << " for $" << highestBid << endl;

        purchaseProperty(board, highestBidder, currentBoardPosition, highestBid);
    } else {
        cout << "No one decided to purchase " << board.boardSpaces.at(currentBoardPosition)->getName()
             << endl;
    }
}








/***** Upgrade Or Downgrade Properties *****/







void GameStateLogic::upgradeProperties(Board &board, int currentPlayer) {
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

void GameStateLogic::downgradeProperties(Board &board, int currentPlayer) {
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

void GameStateLogic::sellUpgradedProperties(Board &board, int currentPlayer, int& currentCashAmount, int rent,  bool &hasUpgrades) {


}

double GameStateLogic::powerFunc(double base, double expo) {
    if(expo <= 0) {
        return 1;
    }

    return base * powerFunc(base, expo - 1);
}
