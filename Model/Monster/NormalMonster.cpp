
#include "NormalMonster.h"


NormalMonster::NormalMonster(string n, int h, int a, int d, int m, vector<ActAction*> acs) : Monster(n, h, a, d, m, acs) 
{

}


int NormalMonster::getActCount() const 
{
    return (int)acts.size();
}

MonsterType NormalMonster::getCategory() const 
{
    return MonsterType::NORMAL;
}