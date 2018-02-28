cmake_minimum_required(VERSION 3.8)
project(hoardingC__)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp)
add_executable(hoardingC__
        Board.cpp
        Board.h
        DiceRoller.cpp
        DiceRoller.h
        GameState.cpp
        GameState.h
        main.cpp
        makefile
        Player.cpp
        Player.h
        Rules.cpp
        Rules.h
        Space.cpp
        Space.h)