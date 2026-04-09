
#ifndef ITEM_H
#define ITEM_H
#include <string>
#include "../Enums.h"
using namespace std;

class Player;

class Item {
private:
    string itemName;
    ItemType type;
    int value;
public:
    Item(string name, ItemType type, int value);
    string getName();
    ItemType getType();
    int getValue();
    void applyEffect(Player& player);
};
#endif