//
// Created by Eben Lester on 12/9/25.
//

#ifndef TILESET_H
#define TILESET_H



#include "Tile.h"

/*  string of Tiles  */

class TileSet : public std::vector<Tile> {

public:
    TileSet();

    static TileSet fullSet(bool red = false);

    friend std::ostream& operator<<(std::ostream& os, const TileSet& tileSet);
};

inline std::ostream& operator<<(std::ostream& os, const TileSet& tileSet) {
    for (Tile t : tileSet)
        os << t;

    return os;
}



#endif //TILESET_H
