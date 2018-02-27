//
// Created by Andrew Eeckman on 1/31/18.
//

#include "Board.h"

using namespace Monopoly;

//**********************************************************************************************************************************

Board::Board() {
    numOfSpaces = 0;
}

//**********************************************************************************************************************************

void Board::readInBoard(char *argv, Board& board) {
    ifstream readIn(argv);

    string tempType;
    string tempName;
    int tempMoneyAroundGo;


    char delim = ',';
    int i = 0;

    if(!readIn.is_open()) {
        cout << "Error opening file" << endl;
    }

    readIn.ignore(256, delim);
    readIn >> numOfSpaces;
    while(i < 4) {
        readIn.ignore(256, '\n');
        i++;
    }

    //Type,Set Id,Intraset Id,Name,Property Cost,House Cost,Hotel Cost,Rent,Rent with House,Rent With Hotel

    for(i = 0; i < numOfSpaces; i++) {

        getline(readIn, tempType, delim);

        if (tempType == "Go") {
            board.boardSpaces.push_back(std::make_unique<goType>(board));
            board.boardSpaces.at(i)->readInGo(readIn);
            tempType.clear();
            continue;

        } else if (tempType == "Property") {
            board.boardSpaces.push_back(std::make_unique<propertyType>(board));
            board.boardSpaces.at(i)->readInProperty(readIn);
            board.boardSpaces.at(i)->setPositionOnBoard(i);
            tempType.clear();
            continue;

        } else if (tempType == "Pay") {
            cout << "READ";
            board.boardSpaces.push_back(std::make_unique<Pay>(board));
            board.boardSpaces.at(i)->readInPay(readIn);
            board.boardSpaces.at(i)->setPositionOnBoard(i);
            tempType.clear();
            continue;

        } else if (tempType == "FreeParking") {
            cout << "READ";
            board.boardSpaces.push_back(std::make_unique<FreeParking>(board));
            board.boardSpaces.at(i)->readInFreeParking(readIn);
            board.boardSpaces.at(i)->setPositionOnBoard(i);
            tempType.clear();
            continue;

        } else if (tempType == "GoToJail") {

        } else if (tempType == "Jail") {

        } else { cout << "!!!ERROR IN READIN!!!"; }

    }
}

//**********************************************************************************************************************************

void Board::displayBoard(Board& board) {
    int count = 0;

    cout << "Space Number| Space Name\t| Owner\t\t| Upgrades\t| Players" << endl;

    for(int i = 0; i < numOfSpaces; i++) {

        board.boardSpaces.at(i)->display();
        cout << endl;
    }
}

//**********************************************************************************************************************************

void Board::initiatePlayers(Board& board) {

    string tempName;

    cout << "Enter how many players will be playing: ";
    cin >> numOfPlayers;
    numOfPlayersLeft = numOfPlayers;

    board.players.assign(static_cast<unsigned long>(numOfPlayers), Player());

    for(int i = 0; i < numOfPlayers; i++) {
        board.players.at(i).setBoardPosition(0);
        board.players.at(i).setNumIdentifier(i);
        board.players.at(i).setCashAmount(rules.getStartingCash());
        board.players.at(i).setInGame(true);
        board.players.at(i).setNetWorth(0);

        board.players.at(i).setPartOfAuction(true);

        int numOfSets = 1;

        for(int id = 1; id < board.getSpaces() - 1; id++) {
            if(board.boardSpaces.at(id+1)->getSetID() > board.boardSpaces.at(id)->getSetID()) {
                numOfSets++;
            }
        }

        cout << "Enter the name of player " << (i + 1) << ": ";
        cin >> tempName;
        board.players.at(i).setName(tempName);
        tempName.clear();
    }
}

//**********************************************************************************************************************************

void Board::readInRules(char *argv) {
    ifstream rulesIn(argv);

    int tempStartingCash;
    int tempTurnLimit;
    int tempNumOfPlayerToEndGame;
    int tempPropertySetMultiplier;
    int tempNumHousesBeforeHotels;
    int tempSalMultiLandingOnGo;

    int tempReRolls;

    string tempBuildEvenly;
    string tempFreeParking;
    string tempAuction;

    if(!rulesIn.is_open()) {
        cout << "Error opening file" << endl;
    }

    rulesIn.ignore(256, ':');

    rulesIn >> tempStartingCash;
    rules.setStartingCash(tempStartingCash);

    rulesIn.ignore(256, '\n');
    rulesIn.ignore(256, ':');

    rulesIn >> tempTurnLimit;
    rules.setTurnLimit(tempTurnLimit);

    rulesIn.ignore(256, '\n');
    rulesIn.ignore(256, ':');

    rulesIn >> tempNumOfPlayerToEndGame;
    rules.setPlayersToEndGame(tempNumOfPlayerToEndGame);

    rulesIn.ignore(256, '\n');
    rulesIn.ignore(256, ':');

    rulesIn >> tempPropertySetMultiplier;
    rules.setSetMulti(tempPropertySetMultiplier);

    rulesIn.ignore(256, '\n');
    rulesIn.ignore(256, ':');

    rulesIn >> tempNumHousesBeforeHotels;
    rules.setHousesBefore(tempNumHousesBeforeHotels);

    rulesIn.ignore(256, '\n');
    rulesIn.ignore(256, ':');

    rulesIn >> tempBuildEvenly;

    if(tempBuildEvenly == "Yes") {
        rules.setBuildEvenly(true);
    }

    rulesIn.ignore(256, '\n');
    rulesIn.ignore(256, ':');

    rulesIn >> tempFreeParking;

    if(tempFreeParking == "Yes") {
        rules.setFreeParking(true);
    }

    rulesIn.ignore(256, '\n');
    rulesIn.ignore(256, ':');

    rulesIn >> tempAuction;

    if(tempAuction == "Yes") {
        rules.setAuction(true);
    }

    rulesIn.ignore(256, '\n');
    rulesIn.ignore(256, ':');

    rulesIn >> tempSalMultiLandingOnGo;
    rules.setSalMultiGo(tempSalMultiLandingOnGo);

    rulesIn.ignore(256, '\n');
    rulesIn.ignore(256, ':');

    rulesIn >> tempReRolls;
    rules.setMaxReRolls(tempReRolls);
}

//**********************************************************************************************************************************
