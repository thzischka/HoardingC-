//
// Created by Andrew Eeckman on 1/31/18.
//

//**********************************************************************************************************************************

#include "Rules.h"

using namespace Monopoly;

//**********************************************************************************************************************************

Rules::Rules() {
    startingCash = 0;
    turnLimit = 0;
    numOfPlayerToEndGame = 0;
    propertySetMultiplier = 0;
    numHousesBeforeHotels = 0;
    buildHousesEvenly = false;         //NOT USING
    putMoneyInFreeParking = false;     //NOT USING
    auctionProperties = false;         //NOT USING
    salMultiLandingOnGo = 0;
}

//**********************************************************************************************************************************

int Rules::getStartingCash() {
    return startingCash;
}

int Rules::getTurnLimit() {
    return turnLimit;
}

int Rules::getPlayersToEndGame() {
    return numOfPlayerToEndGame;
}

int Rules::getSetMulti() {
    return propertySetMultiplier;
}

int Rules::getHousesBefore() {
    return numHousesBeforeHotels;
}

bool Rules::getBuildEvenly() {
    return buildHousesEvenly;
}

bool Rules::getFreeParking() {
    return putMoneyInFreeParking;
}

bool Rules::getAuction() {
    return auctionProperties;
}

int Rules::getSalMultiGo() {
    return salMultiLandingOnGo;
}

int Rules::getMaxReRolls() {
    return maxReRolls;
}

//**********************************************************************************************************************************

void Rules::setStartingCash(int cash) {
    startingCash = cash;
}

void Rules::setTurnLimit(int limit) {
    turnLimit = limit;
}

void Rules::setPlayersToEndGame(int endGameNum) {
    numOfPlayerToEndGame = endGameNum;
}

void Rules::setSetMulti(int setMulti) {
    propertySetMultiplier = setMulti;
}

void Rules::setHousesBefore(int houses) {
    numHousesBeforeHotels = houses;
}

void Rules::setBuildEvenly(bool even) {
    buildHousesEvenly = even;
}

void Rules::setFreeParking(bool free) {
    putMoneyInFreeParking = free;
}

void Rules::setAuction(bool auction) {
    auctionProperties = auction;
}

void Rules::setSalMultiGo(int salMulti) {
    salMultiLandingOnGo = salMulti;
}

void Rules::setMaxReRolls(int numReRolls) {
    maxReRolls = numReRolls;
}


//**********************************************************************************************************************************
