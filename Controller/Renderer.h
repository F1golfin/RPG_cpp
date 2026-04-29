#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <vector>
#include "../Model/Player/Player.h"
#include "../Model/Monster/Monster.h"
#include "../Model/BestiaryEntry.h"

using namespace std;

class Renderer {
public:
    void clearScreen();
    void showMainMenu();
    void showCombatMenu();
    void showPlayerStats(const Player& player);
    void showMonster(const Monster& monster);
    void showInventory(const vector<InventorySlot>& items);
    void showBestiary(const vector<BestiaryEntry>& entries, const string& title);
    void showMessage(const string& text);
    void waitForEnter();
};

#endif
