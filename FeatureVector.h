//
// Created by Eben Lester on 9/15/26.
//

#ifndef FEATUREVECTOR_H
#define FEATUREVECTOR_H



struct FeatureVector {
    bool opens = false,
        honors = false,
        terminals = false,
        simples = false,
        inside_melds = false,
        non_greens = false,
        chis = false,
        tuples = false,
        wind_pair = false,
        dragon_pair = false,
        yakuhai_pair = false;
    unsigned char suits = 0,
        gapped_chis = 0,
        identical_chis = 0,
        unsuited_chis = 0,
        unsuited_tuples = 0,
        closed_tuples = 0,
        wind_tuples = 0,
        dragon_tuples = 0,
        yakuhai_tuples = 0,
        kans = 0;
};



#endif //FEATUREVECTOR_H
