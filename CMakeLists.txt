cmake_minimum_required(VERSION 3.12)
project(comp345)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(comp345
        AggressiveComputer.cpp
        AggressiveComputer.h
        card.cpp
        card.h
        deck.cpp
        deck.h
        dice.h
        game.cpp
        game.h
        HumanPlayer.cpp
        HumanPlayer.h
        main.cpp
        map.cpp
        map.h
        ModerateComputer.cpp
        ModerateComputer.h
        Observer.cpp
        Observer.h
        PhaseObserver.cpp
        PhaseObserver.h
        PhaseSubject.cpp
        PhaseSubject.h
        player.cpp
        player.h
        region.h
        StatObserver.cpp
        StatObserver.h
        StatSubject.cpp
        Strategy.h
        Subject.cpp
        Subject.h
        tile.h
        token.cpp
        token.h StatSubject.h)
