//
// Created by Eben Lester on 10/2/25.
//

#include "Tile.h"

// Throws invalid argument exception if info is invalid.
Tile::Tile(const unsigned char info) : info(info) {
    if ((value() || honor() > WHITE) && (suit() > PINZU || value() > 9))
        throw std::invalid_argument(std::format("{:08b}", info) + " is an invalid Tile.\n");
}

unsigned char Tile::value() const {
    return info & 0x0F;
}

Suit Tile::suit() const {
    return static_cast<Suit>((info & 0x30) >> 4);
}

Honor Tile::honor() const {
    return static_cast<Honor>((info & 0x70) >> 4);
}

bool Tile::bit2() const {
    return (info & 0x40) >> 6;
}

bool Tile::isClosed() const {
    return info & 0x80;
}

bool Tile::isHonor() const {
    return !value();
}

bool Tile::isWind() const {
    return isHonor() && !bit2();
}

bool Tile::isDragon() const {
    return isHonor() && bit2();
}

bool Tile::isTerminal() const {
    return value() == 1 || value() == 9;
}

bool Tile::isGreen() const {
    switch (value()) {
        case 0:
            return honor() == GREEN;
        case 2:
        case 3:
        case 4:
        case 6:
        case 8:
            return suit() == SOUZU;
        default:
            return false;
    }
}

bool Tile::isRed() const {
    return value() && bit2();
}

unsigned char Tile::to_index() const {
    return isHonor() ? honor() : WHITE + 9 * suit() + value();
}

Tile Tile::from_index(int index) {
    if (index < 7)
        return Tile(index << 4);

    index -= 7;
    return Tile((index / 9 << 4) + index % 9);
}

void Tile::open() {
    info |= 0x80;
}

void Tile::close() {
    info &= ~0x80;
}

bool Tile::operator==(const Tile& other) const {
    return to_index() == other.to_index();
}