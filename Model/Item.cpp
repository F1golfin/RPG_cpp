
#include "Item.h"
#include "Player/Player.h"

Item::Item(string name, ItemType type, int value)
    : itemName(name), type(type), value(value)
{

}
string Item::getName() const
{
    return itemName; 
}
ItemType Item::getType() const
{ 
    return type; 
}
int Item::getValue() const
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
