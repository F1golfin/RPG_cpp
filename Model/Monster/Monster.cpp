
#include "Monster.h"


Monster::Monster(string name, int hp, int atk, int def, int mercyGoal, MonsterType category, vector<ActAction*> acts)
    : Entity(name, hp), atk(atk), def(def), mercy(0), mercyGoal(mercyGoal), category(category), acts(acts)
{

}


int Monster::attack(Entity& target) 
{ 
    target.takeDamage(atk); 
    return atk; 
}

void Monster::applyAct(ActAction action) 
{ 
    mercy += action.getMercyImpact(); 
}

bool Monster::canBeMercied() const
{ 
    return mercy >= mercyGoal; 
}

int Monster::getMercy() const
{ 
    return mercy; 
}

int Monster::getMercyGoal() const
{ 
    return mercyGoal; 
}

int Monster::getAtk() const
{ 
    return atk; 
}

int Monster::getDef() const
{ 
    return def; 
}

vector<ActAction*> Monster::getActs() const
{ 
    return acts; 
}

int Monster::getActCount() const
{
    return (int)acts.size();
}

MonsterType Monster::getCategory() const
{
    return category;
}
