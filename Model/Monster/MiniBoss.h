#ifndef MINIBOSS_H
#define MINIBOSS_H

#include "Monster.h"

class MiniBoss : public Monster {
public:
    MiniBoss(string n, int h, int a, int d, int m, vector<ActAction*> acs);
    int getActCount() const override;   // C'est ici qu'on remodifie les fonctions.
    MonsterType getCategory() const override;
};

#endif