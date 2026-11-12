//
// Created by Eben Lester on 10/2/25.
//

#ifndef TILE_H
#define TILE_H



#include <iostream>

/*  A Tile has 8 bits of information describing it:

    [  bit1  ][  bit2  ][     bits3..4     ][               bits5..8               ]
    [hidden? ][red?    ][which suit    0..2][which value                       1..9]
    [hidden? ][which honor             0..6][honor?                               0]

    Bit 1 reflects whether the Tile is hidden.
    Bits 5..8 reflect a Suit Tile's value. If there is no value (0), then it is an Honor Tile.
    If it is an Honor Tile:
        Honors are enumerated 0..6 in bits 2..4 as: East, South, West, North, Red, Green, White.
        Bit 2 reflects whether the Tile is a Dragon. If not, then it is a Wind.
    If it is a Suit Tile:
        Suits are enumerated 0..2 in bits 3..4 as: Character, Bamboo, Circle.
        Bit 2 reflects whether the Tile is red.
*/

enum Honor : unsigned char {
    EAST,
    SOUTH,
    WEST,
    NORTH,
    RED,
    GREEN,
    WHITE
};

enum Suit : unsigned char {
    MANZU, // Character
    SOUZU, // Bamboo
    PINZU  // Circle
};

class Tile {
    unsigned char info;

    bool bit2() const;

public:
    Tile() : info(0x00) {};
    Tile(unsigned char info);
    Tile(const Tile& other) : info(other.info) {};

    Suit suit() const;
    Honor honor() const;
    unsigned char value() const;

    bool isClosed() const;
    bool isHonor() const;
    bool isWind() const;
    bool isDragon() const;
    bool isTerminal() const;
    bool isGreen() const;
    bool isRed() const;
    unsigned char to_index() const;
    static Tile from_index(int index);

    void open();
    void close();

    bool operator==(const Tile& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Tile& tile);
};

inline std::ostream& operator<<(std::ostream& os, const Tile& tile) {
    std::string out = "\xF0\x9F\x80\x80\xEF\xB8\x8E";
    out[3] |= tile.isClosed() ? '\x2B' : tile.to_index();
    os << out;

    return os;
}



#endif //TILE_H
