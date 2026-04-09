
#include "Item.h"
#include "Player/Player.h"

Item::Item(string name, ItemType type, int value)
    : itemName(name), type(type), value(value)
{

}
string Item::getName() 
{
    return itemName; 
}
ItemType Item::getType()  
{ 
    return type; 
}
int Item::getValue()  
{ 
    return value; 
}
void Item::applyEffect(Player& player) 
{ 
    if(type == ItemType::HEAL) 
    {
        player.heal(value); 
    }
}
