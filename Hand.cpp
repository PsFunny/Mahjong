//
// Created by Eben Lester on 8/26/26.
//

#include "Hand.h"

Hand::Hand(const TileSet& tiles) : has_pair(false), num_melds(0), num_tiles(tiles.size()), tiles() {
    for (Tile tile : tiles) {
        this->tiles[tile.to_index()]++;
    }
}

bool Hand::isFull() const {
    return num_tiles + 2 * has_pair + 3 * num_melds == 14;
}

FeatureVector Hand::features(const Honor seat_wind, const Honor game_wind) const {
    FeatureVector features;
    bool manzu = false, souzu = false, pinzu = false;

    if (has_pair) {
        if (pair.isHonor()) {
            features.honors = true;

            if (pair.isDragon()) {
                features.dragon_pair = true;
                features.yakuhai_pair = true;
            }
            else {
                features.wind_pair = true;
                if (pair.honor() == seat_wind || pair.honor() == game_wind) {
                    features.yakuhai_pair = true;
                }
            }
        }
        else {
            switch (pair.suit()) {
                case MANZU:
                    manzu = true;
                    break;
                case SOUZU:
                    souzu = true;
                    break;
                case PINZU:
                    pinzu = true;
                    break;
            }

            if (pair.isTerminal()) {
                features.terminals = true;
            }
            else {
                features.simples = true;
                features.inside_melds = true;
            }
        }

        if (!pair.isGreen()) {
            features.non_greens = true;
        }
    }

    for (int i = 0; i < num_melds; ++i) {
        const Meld& meld = melds[i];

        meld.features(features, seat_wind, game_wind);

        if (!meld.isHonor())  {
            switch (meld.suit()) {
                case MANZU:
                    manzu = true;
                    break;
                case SOUZU:
                    souzu = true;
                    break;
                case PINZU:
                    pinzu = true;
                    break;
            }
        }

        if (meld.type() == CHI) {
            for (int j = i + 1; j < num_melds; ++j) {
                if (melds[j].type() == CHI) {
                    chiFeatures(features, meld, melds[j]);
                }
            }
        }
        else if (!meld.isHonor()) {
            for (int j = i + 1; j < num_melds; ++j) {
                if (melds[j].type() != CHI && meld.value() == melds[j].value()) {
                    features.unsuited_tuples++;
                }
            }
        }
    }

    features.suits = manzu + souzu + pinzu;

    return features;
}

void Hand::chiFeatures(FeatureVector& features, const Meld& chi1, const Meld& chi2) {
    if (chi1.type() != CHI || chi2.type() != CHI) {
        return;
    }

    if (chi1 == chi2) {
        features.identical_chis++;
    }
    else if (chi1.value() == chi2.value()) {
        features.unsuited_chis++;
    }
    else if (chi1.suit() == chi2.suit() && chi1.value() % 3 == 1 && chi2.value() - chi1.value() % 3 == 0) {
        features.gapped_chis++;
    }
}

// Does not include 13-orphans, Blessings, or 9-gates. Those must be checked separately.
unsigned int Hand::yakuman(const FeatureVector& features, const unsigned int waits) {
    unsigned int yakuman = 0;

    if (!features.non_greens) { // All Green
        yakuman++;
    }
    if (features.kans == 4) { // Four Kans
        yakuman++;
    }
    if (features.closed_tuples == 3) { // Four Concealed Pon
        yakuman++;
        if (waits == 1) {
            yakuman++;
        }
    }
    if (!features.simples && !features.terminals) { // All Honors
        yakuman++;
    }
    else if (!features.simples && !features.honors) { // All Terminals
        yakuman++;
    }
    if (features.wind_tuples == 4) { // Big Four Winds
        yakuman += 2;
    }
    else if (features.wind_tuples == 3 && features.wind_pair) { // Little Four Winds
        yakuman++;
    }
    else if (features.dragon_tuples == 3) { // Big Three Dragons
        yakuman++;
    }

    return yakuman;
}

// Does not include 7-pairs, Riichi, or Non-Specific Hand yakus. Those must be checked separately.
unsigned int Hand::yaku(const FeatureVector& features, unsigned int waits) {
    unsigned int han = 0;
    bool yaku = false;

    if (features.suits == 1 && !features.honors) { // Full Flush
        han += 5;;
        if (!features.opens) {
            han++;;
        }
    }
    else if (features.suits == 1) { // Half Flush
        han += 2;
        if (!features.opens) {
            han++;
        }
    }

    if (features.kans == 3) { // Three Kans
        han += 2;
    }
    if (features.unsuited_tuples >= 3) { // Triple Pon
        han += 2;
    }
    if (features.closed_tuples == 3) { // Three Concealed Pon
        han += 2;
    }
    if (!features.chis) { // All Pon
        han += 2;
    }

    if (!features.simples) { // All Terminals or Honors
        han += 2;
    }
    else if (!features.inside_melds && !features.honors) { // Fully Outside Hand
        han += 2;
        if (!features.opens) {
            han++;
        }
    }
    else if (!features.inside_melds) { // Half Outside Hand
        han += 1;
        if (!features.opens) {
            han++;
        }
    }
    if (features.gapped_chis - 2 * features.identical_chis == 3) { // Pure Straight
        han += 1;
        if (!features.opens) {
            han++;
        }
    }
    if (features.unsuited_chis - 2 * features.identical_chis == 3) { // Mixed Triple Chi
        han += 1;
        if (!features.opens) {
            han++;
        }
    }
    if (!features.terminals && !features.honors) { // All Simples
        han += 1;
    }

    if (features.dragon_tuples == 2 && features.dragon_pair) { // Little Three Dragons
        han += 2;
    }
    else { // Yakuhai
        han += features.yakuhai_tuples;
    }

    if (!features.opens && features.identical_chis == 2) { // Twice Pure Double Chi
        han += 3;
    }
    else if (!features.opens && features.unsuited_chis == 2) { // Pure Double Chi
        han += 1;
    }
    if (!features.opens && !features.tuples && !features.yakuhai_pair && waits == 2) { // No Points Hand
        han += 1;
    }
    if (!features.opens) { // Fully Concealed Hand
        han += 1;
    }

    if (han) {
        yaku = true;
    }

    return han;
}

unsigned int Hand::decompose(unsigned char canon[34], const Hand& hand, int idx) {
    while (idx < 34 && canon[idx] == 0)
        ++idx;

    if (idx == 34)
        return 0; //score(hand);

    if (!hand.has_pair && canon[idx] >= 2) {
        Hand decomposition = Hand(hand);

        decomposition.num_tiles -= 2;
        decomposition.counts[idx] -= 2;
        decomposition.has_pair = true;
        decomposition.pair = Tile::from_index(idx);

        decompose(decomposition, idx);
    }

    if (hand.num_melds < 4 && hand.counts[idx] >= 3) {
        Hand new_hand = Hand(hand);

        new_hand.num_tiles -= 3;
        new_hand.counts[idx] -= 3;
        new_hand.melds[new_hand.num_melds++] = Meld(Tile::from_index(idx), PON);

        decompose(new_hand, idx);
    }

    if (const Tile tile = Tile::from_index(idx); hand.num_melds < 4 && tile.value() && tile.value() < 8
        && hand.counts[idx] && hand.counts[idx + 1] && hand.counts[idx + 2]) {
        Hand new_hand = Hand(hand);

        new_hand.num_tiles -= 3;
        new_hand.counts[idx]--;
        new_hand.counts[idx + 1]--;
        new_hand.counts[idx + 2]--;
        new_hand.melds[new_hand.num_melds++] = Meld(tile, CHI);

        decompose(new_hand, idx);
    }
}

Score Hand::score(Hand hand) {
    Score score;

    if (!hand.isFull()) {
        return score;
    }

    bool thirteen_orphans = true,
        seven_pairs = true,
        nine_gates = false;

    score.hand = hand;

    if (hand.num_tiles() == 14) {
        for (int index = 0; index < 34; ++index) {
            if (const Tile tile = Tile::from_index(index); tile.isHonor() || tile.isTerminal()) {
                if (hand.tiles[index] == 0) {
                    thirteen_orphans = false;
                }
            }
            else if (hand.tiles[index] > 0) {
                thirteen_orphans = false;
            }

            if (hand.tiles[index] != 2) {
                seven_pairs = false;
            }
        }

        for (int suit = 0; !nine_gates && suit < 3; ++suit) {
            bool nine_gates_eligible = true;
            int suit_total = 0;

            for (int index = WHITE + 9 * suit; index < WHITE + 9 * (suit + 1); ++index) {
                if (Tile::from_index(index).isTerminal() && hand.tiles[index] < 3 || hand.tiles[index] == 0) {
                    nine_gates_eligible = false;
                }

                suit_total += hand.tiles[index];
            }

            if (nine_gates_eligible && suit_total == 14) {
                nine_gates = true;
            }
        }
    }

    if (thirteen_orphans || nine_gates) {
        score.yakuman++;

        return score;
    }

    if (seven_pairs) {
        score.yaku = true;
        score.han++;
    }

    return decompose(tiles);
}