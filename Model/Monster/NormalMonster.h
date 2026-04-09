#ifndef NORMALMONSTER_H
#define NORMALMONSTER_H

#include "Monster.h"

class NormalMonster : public Monster {
public:
    NormalMonster(string n, int h, int a, int d, int m, vector<ActAction*> acs);
    int getActCount() const override;   // C'est ici qu'on remodifie les fonctions.
    MonsterType getCategory() const override;
};

#endif