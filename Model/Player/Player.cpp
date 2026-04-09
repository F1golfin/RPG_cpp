
#include "Player.h"
#include <iostream>


Player::Player(string name, int maxHp) : Entity(name, maxHp) 
{

}

int Player::attack(Entity& target, int damage) 
{ 
    target.takeDamage(damage); 
    return damage; 
}

void Player::useItem(int index) 
{ 
    inventory.useItem(index, *this); 
}

void Player::addVictory() 
{ 
    victories += 1; 
}

void Player::addKill() 
{ 
    kills += 1; 
}

void Player::addSpare() 
{ 
    spares += 1; 
}

int Player::getVictories()  
{ 
    return victories; 
}

int Player::getKills()  
{ 
    return kills; 
}

int Player::getSpares()  
{ 
    return spares; 
}

Inventory& Player::getInventory() 
{ 
    return inventory; 
}

void Player::getStats() 
{ 
    cout << name << " - HP: " << hp << " / " << maxHp << " | Kills: " << kills << endl; 
}