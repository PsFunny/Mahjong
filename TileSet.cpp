//
// Created by Eben Lester on 12/9/25.
//

#include "TileSet.h"

TileSet TileSet::fullSet(const bool red) {
    TileSet fullSet;

    for (int i = 0; i < 4; i++) {
        for (unsigned char honor = 0x80; honor < 0xF0; honor += 0x10)
            fullSet.emplace_back(honor);
        for (unsigned char suit = 0x80; suit < 0xB0; suit += 0x10)
            for (unsigned char value = 0x01; value < 0x0A; value += 0x01)
                fullSet.emplace_back((red && !i && value == 0x05 ? 0x40 : 0x00) | suit | value);
    }

    return fullSet;
}

