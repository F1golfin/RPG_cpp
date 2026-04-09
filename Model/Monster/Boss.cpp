
#include "Boss.h"


Boss::Boss(string n, int h, int a, int d, int m, vector<ActAction*> acs) : Monster(n, h, a, d, m, acs) 
{

}


int Boss::getActCount() const 
{
    return (int)acts.size();
}

MonsterType Boss::getCategory() const 
{
    return MonsterType::NORMAL;
}