//
// Created by Eben Lester on 9/7/26.
//

#include "Meld.h"

// Throws invalid argument exception if meld is invalid
Meld::Meld(const Tile& base_tile, const MeldType type) : Tile(base_tile), _type(type) {
    if (type == CHI && (base_tile.isHonor() || base_tile.value() > 7))
        throw std::invalid_argument("Invalid Meld.\n");
}

MeldType Meld::type() const {
    return _type;
}

void Meld::features(FeatureVector& features, Honor seat_wind, Honor game_wind) const {
    if (!isClosed()) {
        features.opens = true;
    }

    if (isHonor()) {
        features.honors = true;
    }
    else if (isTerminal()) {
        features.terminals = true;
    }
    else {
        features.simples = true;
        features.inside_melds = true;
    }

    features.non_greens = !isGreen();

    switch (_type) {
        case CHI:
            features.simples = true;
            features.chis = true;
            break;
        case KAN:
            features.kans++;
        case PON:
            if (isClosed()) {
                features.closed_tuples++;
            }

            if (isDragon()) {
                features.dragon_tuples++;
                features.yakuhai_tuples++;
            }
            else if (isWind()) {
                features.wind_tuples++;
                if (honor() == seat_wind || honor() == game_wind) {
                    features.yakuhai_tuples++;
                }
            }

            features.tuples = true;
            break;
    }
}

bool Meld::isTerminal() const {
    return Tile::isTerminal() || _type == CHI && value() == 7;
}

bool Meld::isGreen() const {
    return Tile::isGreen() && (_type != CHI || value() == 2);
}

unsigned int Meld::dora(const Tile& dora) const {
    int diff = to_index() - dora.to_index();

    if (diff == 0) {
        switch (_type) {
            case CHI:
                return 1;
            case KAN:
                return 4;
            case PON:
                return 3;
        }
    }
    else if (_type == CHI && (diff == 1 || diff == 2)) {
        return 1;
    }

    return 0;
}

bool Meld::operator==(const Meld& other) const {
    return Tile::operator==(other) && _type == other._type;
}