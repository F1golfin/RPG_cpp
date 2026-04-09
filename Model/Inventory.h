#ifndef INVENTORY_H
#define INVENTORY_H
#include <vector>
#include "Item.h"
using namespace std;

class InventorySlot {
private:
    Item item;
    int quantity;
public:
    InventorySlot(Item item, int quantity);
    Item getItem() const;
    int getQuantity() const;
    void addQuantity(int amount);
    void removeOne();
    bool isEmpty() const;
};
#endif
