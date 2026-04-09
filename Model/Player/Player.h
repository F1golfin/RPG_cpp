#ifndef PLAYER_H
#define PLAYER_H
#include "../Entity.h"
#include "../Inventory.h"
#include <vector>
using namespace std;

class Player : public Entity {
private:
    int victories = 0;
    int kills = 0;
    int spares = 0;
    vector<InventorySlot> items;
public:
    Player(string name, int maxHp);
    int attack(Entity& target);
    void useItem(int index);
    void addItem(Item item, int quantity);
    void clearItems();
    void displayInventory();
    void addVictory();
    void addKill();
    void addSpare();
    int getVictories() const;
    int getKills() const;
    int getSpares() const;
    const vector<InventorySlot>& getItems() const;
    bool hasItems() const;
    void getStats();
};
#endif
