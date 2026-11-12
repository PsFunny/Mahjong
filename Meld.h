//
// Created by Eben Lester on 9/7/26.
//

#ifndef MELD_H
#define MELD_H



#include "FeatureVector.h"
#include "Tile.h"

enum MeldType : unsigned char {
    CHI,
    KAN,
    PON
};

class Meld : public Tile {
    MeldType _type;

public:
    Meld() : _type(PON) {};
    Meld(const Tile& base_tile, MeldType type);

    MeldType type() const;
    void features(FeatureVector& features, Honor seat_wind, Honor game_wind) const;
    bool isTerminal() const;
    bool isGreen() const;
    unsigned int dora(const Tile& dora) const;

    bool operator==(const Meld& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Meld& meld);
};

inline std::ostream& operator<<(std::ostream& os, const Meld& meld) {
    std::string front = "\xF0\x9F\x80\x80\xEF\xB8\x8E",
        back = "\xF0\x9F\x80\xAB\xEF\xB8\x8E";

    front[3] |= meld.to_index();

    if (meld.isClosed()) {
        switch (meld.type()) {
            case KAN:
                os << back << front << front << back;
                break;
            default:
                os << back << back << back;
                break;
        }
    }
    else {
        switch (meld.type()) {
            case CHI:
                for (int i = 0; i < 3; ++i) {
                    os << front;
                    front[3]++;
                }
                break;
            case KAN:
                os << front;
            case PON:
                os << front << front << front;
                break;
        }
    }

    return os;
}



#endif //MELD_H
