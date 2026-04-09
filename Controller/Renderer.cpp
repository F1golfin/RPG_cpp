#include "Renderer.h"
#include <iostream>
#include <limits>

namespace {
string repeat(char value, int count)
{
    return string(count, value);
}

string monsterTypeLabel(MonsterType type)
{
    if (type == MonsterType::MINIBOSS) {
        return "MINIBOSS";
    }

    if (type == MonsterType::BOSS) {
        return "BOSS";
    }

    return "NORMAL";
}
}

void Renderer::showMainMenu()
{
    cout << repeat('=', 68) << endl;
    cout << "| The Tower of eprajelle" << endl;
    cout << "| [1] Start combat" << endl;
    cout << "| [2] View bestiary" << endl;
    cout << "| [3] View inventory" << endl;
    cout << "| [4] Reload CSV data" << endl;
    cout << "| [5] Exit" << endl;
    cout << repeat('=', 68) << endl;
    cout << "Select an action > ";
}

void Renderer::showCombatMenu()
{
    cout << "[1] Fight  [2] Act  [3] Item  [4] Mercy" << endl;
    cout << "Choose > ";
}

void Renderer::showPlayerStats(const Player& player)
{
    cout << "| " << player.getName()
         << " HP:" << player.getHp() << "/" << player.getMaxHp()
         << " Victories:" << player.getVictories()
         << " Kills:" << player.getKills()
         << " Spares:" << player.getSpares()
         << endl;
}

void Renderer::showMonster(const Monster& monster)
{
    cout << "| " << monster.getName()
         << " [" << monsterTypeLabel(monster.getCategory()) << "]"
         << " HP:" << monster.getHp() << "/" << monster.getMaxHp()
         << " ATK:" << monster.getAtk()
         << " DEF:" << monster.getDef()
         << " MERCY:" << monster.getMercy() << "/" << monster.getMercyGoal()
         << endl;
}

void Renderer::showInventory(const vector<InventorySlot>& items)
{
    cout << repeat('=', 68) << endl;
    cout << "| PLAYER INVENTORY" << endl;

    if (items.empty()) {
        cout << "| Inventory empty." << endl;
        cout << repeat('=', 68) << endl;
        return;
    }

    for (int i = 0; i < (int)items.size(); i++) {
        cout << "| [" << i + 1 << "] "
             << items[i].getItem().getName()
             << " | Qty:" << items[i].getQuantity()
             << " | Value:" << items[i].getItem().getValue()
             << endl;
    }

    cout << repeat('=', 68) << endl;
}

void Renderer::showBestiary(const vector<BestiaryEntry>& entries)
{
    cout << repeat('=', 68) << endl;
    cout << "| BESTIARY ARCHIVE" << endl;

    if (entries.empty()) {
        cout << "| No bestiary entries." << endl;
        cout << repeat('=', 68) << endl;
        return;
    }

    for (const BestiaryEntry& entry : entries) {
        entry.display();
    }

    cout << repeat('=', 68) << endl;
}

void Renderer::showMessage(const string& text)
{
    cout << text << endl;
}

void Renderer::waitForEnter()
{
    cout << endl << "Press ENTER to continue...";
    char character = '\0';

    while (cin.get(character)) {
        if (character == '\n') {
            break;
        }
    }
}
