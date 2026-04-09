#ifndef BOSS_H
#define BOSS_H

#include "Monster.h"

class Boss : public Monster {
public:
    Boss(string n, int h, int a, int d, int m, vector<ActAction*> acs);
    int getActCount() const override;   // C'est ici qu'on remodifie les fonctions.
    MonsterType getCategory() const override;
};

#endif