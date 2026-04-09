#ifndef GAME_H
#define GAME_H

#include "../Model/Player/Player.h"
#include "../Model/Monster/Monster.h"
#include "Renderer.h"
#include "../Model/BestiaryEntry.h"
#include <vector>

using namespace std;

class Game {
private:
    Player player;
    vector<Monster*> monstersPool;
    vector<ActAction> actCatalog;
    vector<BestiaryEntry> bestiary;
    Renderer renderer;
public:
    Game();
    ~Game();
    void loadFiles();
    void loadItems(string fileName);
    void loadMonsters(string fileName);
    void showMenu();
    void startCombat();
    Monster* selectRandomMonster();
    void handleFight(Monster& monster);
    void handleAct(Monster& monster);
    void handleItem();
    void handleMercy(Monster& monster);
    void addBestiaryEntry(Monster monster, bool spared);
    void run();
    EndType getEnding();
};
#endif
