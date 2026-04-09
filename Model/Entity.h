
#ifndef ENTITY_H
#define ENTITY_H
#include <string>
using namespace std;

class Entity {
protected:
    string name;
    int hp;
    int maxHp;
public:
    Entity(string name, int maxHp);
    virtual ~Entity() {}
    string getName() const;
    int getHp() const;
    int getMaxHp() const;
    void takeDamage(int amount);
    void heal(int amount);
    bool isAlive() const;
};
#endif
