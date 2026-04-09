#ifndef BESTIARYENTRY_H
#define BESTIARYENTRY_H
#include <string>
#include "../Enums.h"
using namespace std;

class BestiaryEntry {
private:
    string monsterName;
    MonsterType category;
    int maxHp;
    int atk;
    int def;
    bool spared;
public:
    BestiaryEntry(string name, MonsterType cat, int hp, int a, int d, bool s);
    string getMonsterName() const;
    void setSpared(bool value);
    void display() const;
};
#endif
