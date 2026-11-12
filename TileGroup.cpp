//
// Created by Eben Lester on 12/11/25.
//

#include "TileGroup.h"

unsigned char TileGroup::type() const {
    return info & 0x0C;
}

bool TileGroup::hasTerminal() const {
    const unsigned char value = tile.value();
    return !tile.isHonor() && (value == 0x00 || value == 0x08 || !type() && value == 0x06);
}

bool TileGroup::isValid() const {
    return type() || !tile.isHonor() && tile.value() < 0x07;
}