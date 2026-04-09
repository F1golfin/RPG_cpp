
#include "Inventory.h"


InventorySlot::InventorySlot(Item item, int quantity) : item(item), quantity(quantity) 
{

}


Item InventorySlot::getItem() 
{ 
    return item; 
}

int InventorySlot::getQuantity()  
{ 
    return quantity; 
}

void InventorySlot::addQuantity(int amount) 
{ 
    quantity += amount; 
}

void InventorySlot::removeOne() 
{ 
    if(quantity > 0) 
    {
        quantity -= 1; 
    }
}

bool InventorySlot::isEmpty()
{ 
    return quantity <= 0; 
}


void Inventory::addItem(Item item, int quantity) 
{ 
    items.push_back(InventorySlot(item, quantity)); 

}

void Inventory::useItem(int index, Player& player) // Player le joueur ou le monstre sur lequel on applique l'objet.
{
    if(index >= 0 && index < (int)items.size()) 
    {
        items[index].getItem().applyEffect(player);
        items[index].removeOne();
        if(items[index].isEmpty()) 
        {
            items.erase(items.begin() + index);  // On supprime l'objet s'il y en a plus.
        }
    }
}

bool Inventory::isEmpty()  
{ 
    return items.empty(); 
}

vector<InventorySlot> Inventory::getItems()  
{ 
    return items; 
}