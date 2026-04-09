#ifndef GAME_H
#define GAME_H
#include "../Model/Player/Player.h"
#include "../Model/Monster/Monster.h"
#include "../Model/Monster/NormalMonster.h"
#include "../Model/Monster/MiniBoss.h"
#include "../Model/Monster/Boss.h"
#include "Display.h"
#include "../Model/BestiaryEntry.h"

class Game {
private:
    Player player;
    vector<Monster*> monstersPool;
    vector<ActAction> actCatalog;
    vector<BestiaryEntry> bestiary;
    Display renderer;
    bool filesLoaded;

    void loadActCatalog();
    void loadMonstersCsv(const string& filePath);
    void loadItemsCsv(const string& filePath);
public:
    Game();
    ~Game();
    void loadFiles();
    void showMenu();
    void startCombat();
    void run();
    EndType getEnding();
};
#endif
