
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
    quantity += amount; 
}

void InventorySlot::removeOne() 
{ 
    if(quantity > 0) 
    {
        quantity -= 1; 
    }
}

bool InventorySlot::isEmpty() const
{ 
    return quantity <= 0; 
}
