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
    EncounterStatus status;
public:
    BestiaryEntry(string name, MonsterType cat, int hp, int a, int d, EncounterStatus status);
    string getMonsterName() const;
    MonsterType getCategory() const;
    int getMaxHp() const;
    int getAtk() const;
    int getDef() const;
    EncounterStatus getStatus() const;
    void setStatus(EncounterStatus value);
    void display() const;
};
#endif
