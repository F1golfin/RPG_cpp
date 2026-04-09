#ifndef DISPLAY_H
#define DISPLAY_H
#include <vector>
#include <string>
#include "../Model/Player/Player.h"
#include "../Model/Monster/Monster.h"
#include "../Model/BestiaryEntry.h"
using namespace std;

class Display {
public:
    void showMainMenu();
    void showHeader(const string& title, const string& subtitle = "");
    void showSectionTitle(const string& title);
    void showCombatMenu();
    void showPlayerStats(Player& player);
    void showMonster(Monster& monster);
    void showInventory(Inventory& inventory);
    void showBestiary(const vector<BestiaryEntry>& entries);
    void showMessage(string text);
    void waitForEnter();
};
#endif
