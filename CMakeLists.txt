cmake_minimum_required(VERSION 3.9)
project(hoardingC__)

set(CMAKE_CXX_STANDARD 11)

add_executable(hoardingC__
        Board.cpp
        Board.h
        CampusRules.txt
        DiceRoller.cpp
        DiceRoller.h
        GameState.cpp
        GameState.h
        GameStateLogic.cpp
        GameStateLogic.h
        main.cpp
        makefile
        PayAndFreeParking.csv
        Player.cpp
        Player.h
        RandomNums10000.txt
        Rules.cpp
        Rules.h
        SimpleMonopolyBoard.csv
        Space.cpp
        Space.h)
