
#include "BestiaryEntry.h"
#include <iomanip>
#include <iostream>


BestiaryEntry::BestiaryEntry(string name, MonsterType cat, int hp, int a, int d, EncounterStatus status) 
    : monsterName(name), category(cat), maxHp(hp), atk(a), def(d), status(status)
{
    // On garde une copie simple des informations utiles au bestiaire.
}

string BestiaryEntry::getMonsterName() const
{
    return monsterName;
}

MonsterType BestiaryEntry::getCategory() const
{
    return category;
}

int BestiaryEntry::getMaxHp() const
{
    return maxHp;
}

int BestiaryEntry::getAtk() const
{
    return atk;
}

int BestiaryEntry::getDef() const
{
    return def;
}

EncounterStatus BestiaryEntry::getStatus() const
{
    return status;
}

void BestiaryEntry::setStatus(EncounterStatus value)
{
    // On met a jour le statut quand le joueur gagne contre ce monstre.
    status = value;
}

void BestiaryEntry::display() const
{
    // On laisse chaque entree s'afficher, le Renderer choisit seulement le titre.
    string categoryLabel = "NORMAL";

    if (category == MonsterType::MINIBOSS) {
        categoryLabel = "MINIBOSS";
    } else if (category == MonsterType::BOSS) {
        categoryLabel = "BOSS";
    }

    string statusLabel = "Not encountered";

    // On convertit le statut interne en texte lisible.
    if (status == EncounterStatus::KILLED) {
        statusLabel = "Killed";
    } else if (status == EncounterStatus::SPARED) {
        statusLabel = "Spared";
    }

    cout << "| " << left << setw(28) << monsterName
         << " | " << setw(8) << categoryLabel
         << " | " << right << setw(3) << maxHp
         << " | " << setw(3) << atk
         << " | " << setw(3) << def
         << " | " << left << setw(15) << statusLabel
         << right << " |"
         << endl;
}
