
#include "BestiaryEntry.h"
#include <iostream>


BestiaryEntry::BestiaryEntry(string name, MonsterType cat, int hp, int a, int d, bool s) 
{

}

void BestiaryEntry::display() 
{
    cout << "[" << monsterName << "] HP:" << maxHp << " ATK:" << atk << (spared ? " (Spared)" : " (Killed)") << endl;
}