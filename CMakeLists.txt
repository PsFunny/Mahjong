cmake_minimum_required(VERSION 3.30)
project(Mahjong)

set(CMAKE_CXX_STANDARD 20)

add_executable(Mahjong main.cpp
        Tile.h
        Tile.cpp
        TileSet.cpp
        TileSet.h
        TileGroup.h
        TileGroup.cpp
        Hand.cpp
        Hand.h
        Meld.cpp
        Meld.h
        FeatureVector.h
)
