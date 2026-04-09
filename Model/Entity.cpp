
#include "Entity.h"
#include <algorithm>


Entity::Entity(string name, int maxHp)
    : name(name), hp(maxHp), maxHp(maxHp)
{

}


string Entity::getName() const
{ 
    return name; 
}

int Entity::getHp() const
{ 
    return hp;
}

int Entity::getMaxHp() const
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

bool Entity::isAlive() const
{ 
    return hp > 0; 
}
