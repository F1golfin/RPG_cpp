
#include "Inventory.h"


InventorySlot::InventorySlot(Item item, int quantity) : item(item), quantity(quantity) 
{

}


Item InventorySlot::getItem() const
{ 
    return item; 
}

int InventorySlot::getQuantity() const
{ 
    return quantity; 
}

void InventorySlot::addQuantity(int amount) 
{ 
    // On augmente la quantite si on recupere plusieurs fois le meme item.
    quantity += amount; 
}

void InventorySlot::removeOne() 
{ 
    if(quantity > 0) 
    {
        // On consomme un seul exemplaire quand le joueur utilise l'item.
        quantity -= 1; 
    }
}

bool InventorySlot::isEmpty() const
{ 
    // On sait qu'un slot vide pourra etre retire de l'inventaire.
    return quantity <= 0; 
}
