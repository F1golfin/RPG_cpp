#ifndef PLAYER_H
#define PLAYER_H
#include "../Entity.h"
#include "../Inventory.h"
using namespace std;

class Player : public Entity {
private:
    int victories = 0;
    int kills = 0;
    int spares = 0;
    Inventory inventory;
public:
    Player(string name, int maxHp);
    int attack(Entity& target, int damage);
    void useItem(int index);
    void addVictory();
    void addKill();
    void addSpare();
    int getVictories();
    int getKills();
    int getSpares();
    Inventory& getInventory();
    void getStats();
};
#endif