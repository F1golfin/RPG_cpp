
#include "Player.h"
#include <iostream>


Player::Player(string name, int maxHp) : Entity(name, maxHp) 
{
    // On cree le joueur avec un nom et ses HP maximum.
}

int Player::attack(Entity& target)
{
    // Le sujet proposait des degats aleatoires entre 0 et les HP max.
    // On choisit des degats fixes pour rendre le combat plus lisible.
    const int damage = 10;
    target.takeDamage(damage);
    return damage; 
}

void Player::useItem(int index)
{
    if (index < 0 || index >= (int)items.size()) {
        return;
    }

    // On applique l'effet, puis on retire une quantite de l'inventaire.
    items[index].getItem().applyEffect(*this);
    items[index].removeOne();

    if (items[index].isEmpty()) {
        // On supprime le slot quand il ne reste plus aucun exemplaire.
        items.erase(items.begin() + index);
    }
}

void Player::addItem(Item item, int quantity)
{
    if (quantity <= 0) {
        return;
    }

    // On augmente seulement la quantite si l'item existe deja.
    for (InventorySlot& slot : items) {
        if (slot.getItem().getName() == item.getName()) {
            slot.addQuantity(quantity);
            return;
        }
    }

    // On ajoute une nouvelle ligne dans l'inventaire si l'item est nouveau.
    items.push_back(InventorySlot(item, quantity));
}

void Player::clearItems()
{
    // On vide l'inventaire avant de recharger les items depuis le CSV.
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
