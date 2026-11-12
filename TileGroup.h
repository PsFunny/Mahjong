//
// Created by Eben Lester on 12/11/25.
//

#ifndef TILEGROUP_H
#define TILEGROUP_H



#include "Tile.h"

/*  A Tile Group has a Tile and 4 bits of information describing it:

    [               bits1..4               ][     bits5..6     ][     bits7..8     ]
  	[									   ][type		   0..2][open/pos	   0..3]

   	Types are enumerated 0..2 in bits 5..6 as: Sequence, Triple, Quadruple.
		If it is a Sequence, the Tile is that which has the lowest value in the Sequence.
		If it is a Triple or Quadruple, it is a Group of only the Tile.
	Bits 7..8 describe the origin of the Tile in clockwise order starting at the player.
		If 0, then the completing Tile was drawn, and the group is closed.
		If 1..3, then the group is open.  */

class TileGroup {
    Tile tile;
    unsigned char info;

    unsigned char type() const;

public:
    TileGroup(Tile tile, unsigned char info) : tile(tile), info(info) {}

    bool hasTerminal() const;
	bool isValid() const;
};



#endif //TILEGROUP_H
