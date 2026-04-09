
#include "BestiaryEntry.h"
#include <iostream>


BestiaryEntry::BestiaryEntry(string name, MonsterType cat, int hp, int a, int d, bool s) 
    : monsterName(name), category(cat), maxHp(hp), atk(a), def(d), spared(s)
{

}

string BestiaryEntry::getMonsterName() const
{
    return monsterName;
}

void BestiaryEntry::setSpared(bool value)
{
    spared = value;
}

void BestiaryEntry::display() const
{
    string categoryLabel = "NORMAL";

    if (category == MonsterType::MINIBOSS) {
        categoryLabel = "MINIBOSS";
    } else if (category == MonsterType::BOSS) {
        categoryLabel = "BOSS";
    }

    cout << "| " << monsterName
         << " [" << categoryLabel << "]"
         << " HP:" << maxHp
         << " ATK:" << atk
         << " DEF:" << def
         << " STATUS:" << (spared ? "SPARED" : "UNKNOWN")
         << endl;
}
