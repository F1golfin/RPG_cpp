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
    Item getItem();
    int getQuantity();
    void addQuantity(int amount);
    void removeOne();
    bool isEmpty();
};

class Inventory {
private:
    vector<InventorySlot> items;
public:
    void addItem(Item item, int quantity);
    void useItem(int index, Player& player);
    void display();
    bool isEmpty();
    vector<InventorySlot> getItems();
};
#endif