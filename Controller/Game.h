#ifndef GAME_H
#define GAME_H
#include "../Model/Player/Player.h"
#include "../Model/Monster/Monster.h"
#include "Display.h"
#include "../Model/BestiaryEntry.h"

class Game {
private:
    Player player;
    vector<Monster*> monstersPool;
    vector<ActAction> actCatalog;
    vector<BestiaryEntry> bestiary;
    Display renderer;
public:
    Game();
    void loadFiles();
    void showMenu();
    void startCombat();
    void run();
    EndType getEnding();
};
#endif