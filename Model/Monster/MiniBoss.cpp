
#include "MiniBoss.h"


MiniBoss::MiniBoss(string n, int h, int a, int d, int m, vector<ActAction*> acs) : Monster(n, h, a, d, m, acs) 
{

}


int MiniBoss::getActCount() const 
{
    return (int)acts.size();
}

MonsterType MiniBoss::getCategory() const 
{
    return MonsterType::MINIBOSS;
}
