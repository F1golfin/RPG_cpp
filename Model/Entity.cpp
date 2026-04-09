
#include "Entity.h"
#include <algorithm>


Entity::Entity(string name, int maxHp)
    : name(name), hp(maxHp), maxHp(maxHp)
{

}


string Entity::getName()  
{ 
    return name; 
}

int Entity::getHp()
{ 
    return hp;
}

int Entity::getMaxHp()
{ 
    return maxHp;
}

void Entity::takeDamage(int amount) 
{ 
    hp = max(0, hp - amount); 
}

void Entity::heal(int amount) 
{ 
    hp = min(maxHp, hp + amount); 
}

bool Entity::isAlive()  
{ 
    return hp > 0; 
}
