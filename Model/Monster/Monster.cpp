
#include "Monster.h"


Monster::Monster(string name, int hp, int atk, int def, int mercyGoal, vector<ActAction*> acts): Entity(name, hp), atk(atk), def(def), mercy(0), mercyGoal(mercyGoal), acts(acts) 
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

bool Monster::canBeMercied() 
{ 
    return mercy >= mercyGoal; 
}

int Monster::getMercy()  
{ 
    return mercy; 
}

int Monster::getMercyGoal()  
{ 
    return mercyGoal; 
}

int Monster::getAtk()  
{ 
    return atk; 
}

int Monster::getDef()  
{ 
    return def; 
}

vector<ActAction*> Monster::getActs()  
{ 
    return acts; 
}
