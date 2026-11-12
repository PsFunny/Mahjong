//
// Created by Eben Lester on 10/2/25.
//

#include <stdio.h>
#include "TileSet.h"

int main() {
    TileSet wall = TileSet::fullSet();
    TileSet deadWall, hands[4], rivers[4];

    for (int i = 0; i < 14; ++i) {
        deadWall.push_back(wall.drawRandom());
    }
    deadWall[0].flip();

    for (TileSet& hand : hands)
        for (int i = 0; i < 13; ++i)
            hand.push_back(wall.drawRandom());
    for (int i = 0; i < 13; ++i)
        hands[0][i].flip();

    std::cout << "Dead Wall: " << deadWall << std::endl;
    std::cout << "Wall: " << wall << std::endl;
    for (TileSet& hand : hands)
        std::cout << hand << std::endl;

    return 0;
}