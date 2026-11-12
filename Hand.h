//
// Created by Eben Lester on 8/26/26.
//

#ifndef HAND_H
#define HAND_H



#include "FeatureVector.h"
#include "Meld.h"
#include "TileSet.h"

struct Score;

class Hand {
    bool has_pair;
    Tile pair;
    unsigned char num_melds;
    Meld melds[4];
    unsigned char num_tiles,
        tiles[34];

    static void chiFeatures(FeatureVector& features, const Meld& chi1, const Meld& chi2);
    static unsigned int decompose(unsigned char canon[34], const Hand& hand, int idx = 0);

public:
    Hand() : has_pair(false), num_melds(0),  num_tiles(0), tiles() {}
    Hand(const TileSet& tiles);

    bool isFull() const;
    FeatureVector features(Honor seat_wind, Honor game_wind) const;
    static unsigned int yakuman(const FeatureVector& features, unsigned int waits);
    static unsigned int yaku(const FeatureVector& features, unsigned int waits);

    static Score score(Hand hand);
};

struct Score {
    Hand hand;
    bool yaku = false;
    unsigned char han = 0,
        yakuman = 0;
};



#endif //HAND_H
