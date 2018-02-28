//
// Created by Andrew Eeckman on 2/7/18.
//

#include "Space.h"
#include "Board.h"

using namespace Monopoly;


/***** BOARDSPACE *****/

boardSpace::boardSpace() {

}
void boardSpace::setType(const string &typeIn) { }
void boardSpace::setSetID(int set) { }
void boardSpace::setIntraID(int intra) { }
void boardSpace::setName(const string &nameIn) { }
void boardSpace::setPropertyCost(int cost) { }
void boardSpace::setHouseCost(int hCost) { }
void boardSpace::setHotelCost(int htCost) { }
void boardSpace::setStartRent(int sR) { }
void boardSpace::setRent(int r){ } 
void boardSpace::setRentHouse(int hRent){ } 
void boardSpace::setRentHotel(int htRent){ } 
void boardSpace::setOwned(bool o){ } 
void boardSpace::setOwnedBy(int by){ } 
void boardSpace::setPositionOnBoard(int pos){ } 
void boardSpace::setNumHouses(int numH){ } 
void boardSpace::setNumHotels(int numHot){ } 

string boardSpace::getType(){ return "ERROR"; } 
int boardSpace::getSetID(){ return -99; } 
int boardSpace::getIntraID(){ return -99; } 
string boardSpace::getName(){ return "ERROR"; } 
int boardSpace::getPropertyCost(){ return -99; } 
int boardSpace::getHouseCost(){ return -99; } 
int boardSpace::getHotelCost(){ return -99; } 
int boardSpace::getStartRent(){ return -99; } 
int boardSpace::getRent(){ return -99; } 
int boardSpace::getRentHouse(){ return -99; } 
int boardSpace::getRentHotel(){ return -99; } 
bool boardSpace::getOwned(){ return false; } 
int boardSpace::getOwnedBy(){ return -99; } 
int boardSpace::getPositionOnBoard(){ return -99; } 
int boardSpace::getNumHouses(){ return -99; } 
int boardSpace::getNumHotels(){ return -99; } 
bool boardSpace::getCanHouse(){ return false; } 

/***** PROPERTY *****/

propertyType::propertyType(Board &board) : board(board) {
    type.assign("Property");
    name.assign(" ");
    propertyCost = 0;
    houseCost = 0;
    hotelCost = 0;
    rent = 0;
    rentWithHouse = 0;
    rentWithHotel = 0;
    numHouses = 0;
    numHotels = 0;

}

void propertyType::activate(Player &activatingPlayer) {

    int currentCashAmount = activatingPlayer.getCashAmount();
    int currentBoardPosition = activatingPlayer.getBoardPosition();
    int rentOfPos = rent;
    char choice = ' ';

    if (owned) {
        if (ownedBy == activatingPlayer.getNumIdentifier()) {
            //board.boardSpaces.at(currentBoardPosition)->activate(board.players.at(currentPlayer));
        } else {
            int setMulti = 1; //Checks to see if the property set is owned by one person

            for (int position = 1; position < board.getSpaces(); position++) {
                if (board.boardSpaces.at(position)->getSetID() == setID) {
                    if (ownedBy == board.boardSpaces.at(position)->getOwnedBy()) {
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
                    if (board.boardSpaces.at(num)->getOwnedBy() == activatingPlayer.getNumIdentifier()) {
                        if(board.boardSpaces.at(num)->getNumHotels() != 0 || board.boardSpaces.at(num)->getNumHouses() != 0 ) {
                            hasUpgrades = true;
                        }
                    }
                }
                while ((currentCashAmount - (rentOfPos * setMulti)) < 0 && hasUpgrades ) {
                    bool canDegrade = false;
                    std::vector <int> degradable;

                    for (unsigned int i = 0; i <  board.boardSpaces.size(); i++) { //Cycles throught the properties looking for the upgradeable ones
                        canDegrade = true; //reset the value before execution of loop
                        if (board.boardSpaces.at(i)->getNumHouses() == 0 && board.boardSpaces.at(i)->getNumHotels() != 1) {
                            canDegrade = true;
                            continue;
                        }
                        if (board.boardSpaces.at(i)->getOwnedBy() == activatingPlayer.getNumIdentifier()) {
                            for(unsigned int j = 0; j < board.boardSpaces.size(); j++){
                                if(board.boardSpaces.at(i)->getSetID() == board.boardSpaces.at(j)->getSetID()){
                                    if(board.rules.getBuildEvenly()) {
                                        if(board.boardSpaces.at(i)->getNumHotels() == 1) {
                                            //Just to make sure that it can catch if a propety has a hotel and no houses
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

                    } else {
                        cout << "You have $" << activatingPlayer.getCashAmount() << " but you owe $" << (rent) << endl;
                        cout << "Pick an upgrade to sell to make up the difference" << endl;
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

                            changeCashAmount(board, activatingPlayer.getNumIdentifier(), (currentCashAmount + (board.boardSpaces.at(degradable.at(userChoice))->getHouseCost()/2)), currentCashAmount);

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
                            changeCashAmount(board, activatingPlayer.getNumIdentifier(), (currentCashAmount + (board.boardSpaces.at(degradable.at(userChoice))->getHotelCost()/2)), currentCashAmount);
                            board.boardSpaces.at(degradable.at(userChoice))->setRent(board.boardSpaces.at(degradable.at(userChoice))->getRentHouse() * (int)powerFunc(2, (board.boardSpaces.at(degradable.at(userChoice))->getNumHouses() - 1)));
                        }
                    }
                    degradable.clear();
                    for (unsigned int num = 0 ; num < board.boardSpaces.size(); num++) {
                        if ( board.boardSpaces.at(num)->getOwnedBy() == activatingPlayer.getNumIdentifier()) {
                            if(board.boardSpaces.at(num)->getNumHotels() != 0 || board.boardSpaces.at(num)->getNumHouses() != 0 ) {
                                hasUpgrades = true;
                                return;
                            } else {
                                hasUpgrades = false;
                                return;
                            }
                        }
                    }
                }

                if ((currentCashAmount - (rentOfPos * setMulti)) < 0) {

                    cout << activatingPlayer.getName() << " went bankrupt to " <<
                         board.players.at(ownedBy).getName()
                         << " for landing on " << name
                         << endl;

                    changeCashAmount(board, ownedBy, (board.players.at(ownedBy).getCashAmount() + currentCashAmount), currentCashAmount);

                    for (int i = 1; i < board.getSpaces(); i++) {
                        if (board.boardSpaces.at(i)->getOwnedBy()
                            == activatingPlayer.getNumIdentifier()) {
                            board.boardSpaces.at(i)->setOwnedBy(board.players.at(ownedBy).getNumIdentifier());
                        }
                    }

                    //leaveGame(board, currentPlayer);
                    activatingPlayer.setInGame(false);
                } else {
                    changeCashAmount(board, activatingPlayer.getNumIdentifier(), (currentCashAmount -= (rentOfPos * setMulti)), currentCashAmount);

                    board.players.at(ownedBy).setCashAmount(
                            board.players.at(ownedBy).getCashAmount()
                            + (rentOfPos) * setMulti
                    );

                    cout << activatingPlayer.getName() << " paid " <<
                         board.players.at(ownedBy).getName() <<
                         " $" << (rentOfPos * setMulti) << " for landing on "
                         << name << endl;
                }

            } else {
                int holderVariable = 0;
                changeCashAmount(board, activatingPlayer.getNumIdentifier(), (currentCashAmount -= (rentOfPos * setMulti)), currentCashAmount);

                board.players.at(ownedBy).setCashAmount(
                        board.players.at(ownedBy).getCashAmount()
                        + (rentOfPos) * setMulti
                );

                changeCashAmount(board, ownedBy, (board.players.at(ownedBy).getCashAmount() + (rentOfPos) * setMulti), holderVariable);

                cout << activatingPlayer.getName() << " paid " <<
                     board.players.at(ownedBy).getName() <<
                     " $" << (rentOfPos * setMulti) << " for landing on "
                     << name << endl;
            }
        }

    } else if(!ownedBy) {

        if(propertyCost > activatingPlayer.getCashAmount()) {

            cout << activatingPlayer.getName() << ", you don't have enough money to afford " <<
                 name << "." << endl;

            cout << name << " costs $" << propertyCost << " but you only have $" <<
                 activatingPlayer.getCashAmount() << endl;

        } else {

            cout << "Would you like to buy " << name << " for $" << propertyCost << "?" << endl;

            cout << "Rent on " << name << " is $" << rent << endl;

            cout << "Enter y for yes or n for no: " << endl;

            cin >> choice;
        }

        if (choice == 'Y' || choice == 'y') {

            int currentCashAmount = activatingPlayer.getCashAmount();

            purchaseProperty(board, activatingPlayer.getNumIdentifier(), currentBoardPosition, propertyCost);

            changeCashAmount(board, activatingPlayer.getNumIdentifier(), (currentCashAmount - propertyCost), currentCashAmount);

            owned = true;

            ownedBy = activatingPlayer.getNumIdentifier();

            activatingPlayer.setNetWorth(activatingPlayer.getNetWorth() + propertyCost);

            cout << activatingPlayer.getName() << " bought " << name << " for $" << propertyCost << endl;
        }

        else if(choice == 'N' || choice == 'n') {
            if(board.rules.getAuction()) {
                auctionProperty(board, activatingPlayer.getNumIdentifier(), currentBoardPosition);
            }

            for(int i = 0; i < board.getNumOfPlayersLeft(); i++) {
                if(board.players.at(i).getInGame()) {
                    board.players.at(i).setPartOfAuction(true);
                }
            }
        }
    }
}

void propertyType::readInProperty(ifstream &readIn) {

    type = "Property";

    readIn >> setID;

    readIn.ignore(256, ',');

    readIn >> intraID;

    readIn.ignore(256, ',');

    getline(readIn, name, ',');

    readIn >> propertyCost;

    readIn.ignore(256, ',');

    readIn >> houseCost;

    readIn.ignore(256, ',');

    readIn >> hotelCost;

    readIn.ignore(256, ',');

    readIn >> startRent;
    rent = startRent;

    readIn.ignore(256, ',');

    readIn >> rentWithHouse;

    readIn.ignore(256, ',');

    readIn >> rentWithHotel;

    readIn.ignore(256, '\n');

    owned = false;
    ownedBy = -99;
}

void propertyType::display() {
    int count = 0;
    cout << positionOnBoard << "\t\t\t| " << name << "\t\t\t\t| ";

    if(owned) {

        cout << board.players.at(ownedBy).getName() << "\t| ";

    } else {
        cout << "None\t\t| ";
    }

    for(int h = 0; h < numHouses; h ++){
        cout << "h";
    }
    if (numHotels == 1){
        cout << "H";
    }
    cout << "\t\t\t| ";

    for(int j = 0; j < board.getPlayerNumber(); j++){
        if(board.players.at(j).getBoardPosition() == positionOnBoard) {
            if(count > 0) {
                cout << ", ";
            }
            cout << "[" << board.players.at(j).getName() << " : $" << board.players.at(j).getCashAmount() << "]";
            count++;
        }
    }

    count = 0;
}

/***** GO *****/

goType::goType(Board &board) : board(board) {
    type.assign("Go");
    moneyAroundGo = 0;
    name.assign(" ");
}

void goType::readInGo(ifstream &readIn) {

    Rules rules;

    type = "Go";

    readIn >> moneyAroundGo;

    readIn.ignore(256, ',');

    getline(readIn, name, ',');

    readIn.ignore(256, '\n');

    salMultiAroundGo = rules.getSalMultiGo();
}

void goType::activate(Player &activatingPlayer) {
    int currentCashAmount = activatingPlayer.getCashAmount();
    activatingPlayer.setCashAmount(currentCashAmount + (moneyAroundGo * salMultiAroundGo));
}

void goType::display() {

    int count = 0;

    cout << positionOnBoard << "\t\t\t| " << name << "\t\t\t| None\t\t|         \t| ";

    for(int j = 0; j < board.getPlayerNumber(); j++){
        if(board.players.at(j).getBoardPosition() == positionOnBoard) {
            if (count >= 1) {
                cout << ", ";
            }
            cout << "[" << board.players.at(j).getName() << " : $" << board.players.at(j).getCashAmount() << "] ";
            count++;
        }
    }
}

/***** FREE PARKING *****/

FreeParking::FreeParking(Board &board) : board(board) {
    type = "FreeParking";
    name = "ERROR";
}


void FreeParking::activate(Player &activatingPlayer) {
    int moneyInBank = board.getMoneyInFreeParking();
    cout << "Money In Bank: " << moneyInBank << endl;

    int currentCashAmount = activatingPlayer.getCashAmount();

    if(board.rules.getFreeParking()) {
        currentCashAmount = currentCashAmount + moneyInBank;
        activatingPlayer.setCashAmount(currentCashAmount);

        moneyInBank = 0;
        board.setMoneyInFreeParking(moneyInBank);
    }
}

void FreeParking::display() {
    int count = 0;

    cout << positionOnBoard << "\t\t\t| " << name << "\t\t\t| None\t\t|         \t| ";

    for(int j = 0; j < board.getPlayerNumber(); j++){
        if(board.players.at(j).getBoardPosition() == positionOnBoard) {
            if (count >= 1) {
                cout << ", ";
            }
            cout << "[" << board.players.at(j).getName() << " : $" << board.players.at(j).getCashAmount() << "] ";
            count++;
        }
    }
}

void FreeParking::readInFreeParking(ifstream &readIn) {

    type = "FreeParking";

    getline(readIn, name, ',');

    readIn.ignore(256, '\n');
}

/***** PAY *****/

Pay::Pay(Board &board) : board(board) {
    type = "Pay";
    name = "ERROR";
    moneyToBePaid = 0;
}

void Pay::activate(Player &activatingPlayer) {
    int moneyInBank = board.getMoneyInFreeParking();
    int currentCashAmount = activatingPlayer.getCashAmount();
    int currentBoardPosition = activatingPlayer.getBoardPosition();

    if((currentCashAmount - moneyToBePaid) >= 0) {
        if(board.rules.getFreeParking()) {
            board.setMoneyInFreeParking(moneyInBank + moneyToBePaid);
        }
        activatingPlayer.setCashAmount(currentCashAmount - moneyToBePaid);

        cout << activatingPlayer.getName() << " paid the bank $" << moneyToBePaid << " for landing on "
             << name << endl;

    } else {

        bool hasUpgrades = false;

        for (unsigned int num = 0 ; num < board.boardSpaces.size(); num++) {
            if (board.boardSpaces.at(num)->getOwnedBy() == activatingPlayer.getNumIdentifier()) {
                if(board.boardSpaces.at(num)->getNumHotels() != 0 || board.boardSpaces.at(num)->getNumHouses() != 0) {
                    hasUpgrades = true;
                }
            }
        }
        while ((currentCashAmount - moneyToBePaid) < 0 && hasUpgrades ) {
            bool canDegrade = false;
            std::vector <int> degradable;

            for (unsigned int i = 0; i <  board.boardSpaces.size(); i++) { //Cycles throught the properties looking for the upgradeable ones
                canDegrade = true; //reset the value before execution of loop
                if (board.boardSpaces.at(i)->getNumHouses() == 0 && board.boardSpaces.at(i)->getNumHotels() != 1) {
                    canDegrade = true;
                    continue;
                }
                if (board.boardSpaces.at(i)->getOwnedBy() == activatingPlayer.getNumIdentifier()) {
                    for(unsigned int j = 0; j < board.boardSpaces.size(); j++){
                        if(board.boardSpaces.at(i)->getSetID() == board.boardSpaces.at(j)->getSetID()){
                            if(board.rules.getBuildEvenly()) {
                                if(board.boardSpaces.at(i)->getNumHotels() == 1) {
                                    //Just to make sure that it can catch if a propety has a hotel and no houses
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

            } else {
                cout << "You have $" << activatingPlayer.getCashAmount() << " but you owe $" << moneyToBePaid << endl;
                cout << "Pick an upgrade to sell to make up the difference" << endl;
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

                    changeCashAmount(board, activatingPlayer.getNumIdentifier(), (currentCashAmount + (board.boardSpaces.at(degradable.at(userChoice))->getHouseCost()/2)), currentCashAmount);

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
                    changeCashAmount(board, activatingPlayer.getNumIdentifier(), (currentCashAmount + (board.boardSpaces.at(degradable.at(userChoice))->getHotelCost()/2)), currentCashAmount);
                    board.boardSpaces.at(degradable.at(userChoice))->setRent(board.boardSpaces.at(degradable.at(userChoice))->getRentHouse() * (int)powerFunc(2, (board.boardSpaces.at(degradable.at(userChoice))->getNumHouses() - 1)));
                }
            }
            degradable.clear();
            for (unsigned int num = 0 ; num < board.boardSpaces.size(); num++) {
                if ( board.boardSpaces.at(num)->getOwnedBy() == activatingPlayer.getNumIdentifier()) {
                    if(board.boardSpaces.at(num)->getNumHotels() != 0 || board.boardSpaces.at(num)->getNumHouses() != 0 ) {
                        hasUpgrades = true;
                        return;
                    } else {
                        hasUpgrades = false;
                        return;
                    }
                }
            }
        }

        if ((currentCashAmount - moneyToBePaid) < 0) {

            cout << activatingPlayer.getName() << " went bankrupt to the bank for landing on "
                 << name << endl;

            board.setMoneyInFreeParking(moneyInBank + activatingPlayer.getCashAmount());
            activatingPlayer.setCashAmount(0);

            for (int i = 1; i < board.getSpaces(); i++) {
                if (board.boardSpaces.at(i)->getOwnedBy()
                    == activatingPlayer.getNumIdentifier()) {
                    board.boardSpaces.at(i)->setOwnedBy(board.players.at(
                            board.boardSpaces[currentBoardPosition]->getOwnedBy()).getNumIdentifier());
                }
            }

            activatingPlayer.setInGame(false);
        } else {
            changeCashAmount(board, activatingPlayer.getNumIdentifier(), (currentCashAmount -= moneyToBePaid), currentCashAmount);

            board.setMoneyInFreeParking(moneyInBank + moneyToBePaid);

            cout << activatingPlayer.getName() << " paid the bank $" << moneyToBePaid << " for landing on "
                 << name << endl;
        }
    }
}

void Pay::display() {
    int count = 0;

    cout << positionOnBoard << "\t\t\t| " << name << "\t\t\t| None\t\t|         \t| ";

    for(int j = 0; j < board.getPlayerNumber(); j++){
        if(board.players.at(j).getBoardPosition() == positionOnBoard) {
            if (count >= 1) {
                cout << ", ";
            }
            cout << "[" << board.players.at(j).getName() << " : $" << board.players.at(j).getCashAmount() << "] ";
            count++;
        }
    }
}

void Pay::readInPay(ifstream &readIn) {

    type = "Pay";

    getline(readIn, name, ',');

    readIn >> moneyToBePaid;

    readIn.ignore(256, '\n');
}


/***** Change Positioning Or Cash Value Amounts *****/


void SpaceFunc::changeCashAmount(Board &board, int currentPlayer, int changeInVal, int &currentCashAmount) {
    board.players.at(currentPlayer).setCashAmount(changeInVal);
    currentCashAmount = board.players.at(currentPlayer).getCashAmount();
}

/***** Purchase Or Auction Off Properties *****/

void SpaceFunc::purchaseProperty(Board &board, int currentPlayer, int currentBoardPosition, int amountToPay) {

    int currentCashAmount = board.players.at(currentPlayer).getCashAmount();

    changeCashAmount(board, currentPlayer, (currentCashAmount - amountToPay), currentCashAmount);
    board.boardSpaces.at(currentBoardPosition)->setOwned(true);
    board.boardSpaces.at(currentBoardPosition)->setOwnedBy(board.players.at(currentPlayer).getNumIdentifier());
    board.players.at(currentPlayer).setNetWorth(
            board.players.at(currentPlayer).getNetWorth() + board.boardSpaces.at(currentBoardPosition)->getPropertyCost()
    );
}

void SpaceFunc::auctionProperty(Board &board, int currentPlayer, int currentBoardPosition) {

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

void SpaceFunc::sellUpgradedProperties(Board &board, int currentPlayer, int& currentCashAmount, int rent,  bool &hasUpgrades) {
    //FIXME OR DON'T
}

/***** Power Function *****/

double SpaceFunc::powerFunc(double base, double expo) {
    if(expo <= 0) {
        return 1;
    }

    return base * powerFunc(base, expo - 1);
}
