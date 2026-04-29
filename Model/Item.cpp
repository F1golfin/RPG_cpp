
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
        // On applique l'effet de soin directement sur le joueur.
        player.heal(value); 
    } else if (type == ItemType::BUFF_ATTACK) {
        // On applique un bonus d'attaque pour rendre certains items offensifs.
        player.addAttackBonus(value);
    } else if (type == ItemType::BUFF_DEFENSE) {
        // On applique un bonus de defense pour reduire les degats recus.
        player.addDefenseBonus(value);
    }
}
