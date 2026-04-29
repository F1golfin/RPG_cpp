
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

void Entity::setName(string value)
{
    name = value;
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
    // On empeche les HP de passer sous 0 pour eviter les valeurs negatives.
    hp = max(0, hp - amount); 
}

void Entity::heal(int amount) 
{ 
    // On soigne sans depasser les HP maximum de l'entite.
    hp = min(maxHp, hp + amount); 
}

bool Entity::isAlive() const
{ 
    // On considere une entite vivante tant qu'il lui reste au moins 1 HP.
    return hp > 0; 
}
