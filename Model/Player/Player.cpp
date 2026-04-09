
#include "Player.h"
#include <iostream>


Player::Player(string name, int maxHp) : Entity(name, maxHp) 
{

}

int Player::attack(Entity& target)
{
    const int damage = 10;
    target.takeDamage(damage);
    return damage; 
}

void Player::useItem(int index)
{
    if (index < 0 || index >= (int)items.size()) {
        return;
    }

    items[index].getItem().applyEffect(*this);
    items[index].removeOne();

    if (items[index].isEmpty()) {
        items.erase(items.begin() + index);
    }
}

void Player::addItem(Item item, int quantity)
{
    if (quantity <= 0) {
        return;
    }

    for (InventorySlot& slot : items) {
        if (slot.getItem().getName() == item.getName()) {
            slot.addQuantity(quantity);
            return;
        }
    }

    items.push_back(InventorySlot(item, quantity));
}

void Player::clearItems()
{
    items.clear();
}

void Player::displayInventory()
{
    if (items.empty()) {
        cout << "Inventory empty." << endl;
        return;
    }

    for (int i = 0; i < (int)items.size(); i++) {
        cout << "[" << i + 1 << "] "
             << items[i].getItem().getName()
             << " x" << items[i].getQuantity()
             << endl;
    }
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

int Player::getVictories() const
{ 
    return victories; 
}

int Player::getKills() const
{ 
    return kills; 
}

int Player::getSpares() const
{ 
    return spares; 
}

const vector<InventorySlot>& Player::getItems() const
{
    return items;
}

bool Player::hasItems() const
{
    return !items.empty();
}

void Player::getStats() 
{ 
    cout << name
         << " - HP: " << hp << " / " << maxHp
         << " | Victories: " << victories
         << " | Kills: " << kills
         << " | Spares: " << spares
         << endl;
}
