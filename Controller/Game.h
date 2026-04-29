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
    vector<InventorySlot> itemPool;
    vector<ActAction> actCatalog;
    vector<BestiaryEntry> completeBestiary;
    vector<BestiaryEntry> combatHistory;
    Renderer renderer;
public:
    Game();
    ~Game();
    void loadFiles();
    void loadItems(string fileName);
    void loadMonsters(string fileName);
    void addItemToPool(Item item, int quantity);
    bool grantItemFromPool(int index);
    void giveStartingItems();
    void tryDropItem(MonsterType category);
    void askPlayerName();
    void showMenu();
    void showEnding();
    void startCombat();
    Monster* selectRandomMonster();
    void handleFight(Monster& monster);
    void handleAct(Monster& monster);
    void handleItem();
    void handleMercy(Monster& monster);
    void recordCombatResult(const Monster& monster, EncounterStatus status);
    void run();
    EndType getEnding();
};
#endif
