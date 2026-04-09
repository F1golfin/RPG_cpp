#include "Game.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>

namespace {
vector<string> splitCsvLine(const string& line)
{
    vector<string> tokens;
    string token;
    stringstream stream(line);

    while (getline(stream, token, ';')) {
        tokens.push_back(token);
    }

    return tokens;
}

MonsterType parseMonsterType(const string& value)
{
    if (value == "NORMAL") {
        return MonsterType::NORMAL;
    }
    if (value == "MINIBOSS") {
        return MonsterType::MINIBOSS;
    }
    if (value == "BOSS") {
        return MonsterType::BOSS;
    }

    throw runtime_error("Unknown monster type: " + value);
}

ItemType parseItemType(const string& value)
{
    if (value == "HEAL") {
        return ItemType::HEAL;
    }

    throw runtime_error("Unknown item type: " + value);
}

int readInt()
{
    int value = 0;
    cin >> value;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return -1;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return value;
}
}

Game::Game() : player("Frisk", 20)
{
    srand((unsigned int)time(nullptr));

    actCatalog.push_back(ActAction("OBSERVE", "Observe", 15));
    actCatalog.push_back(ActAction("COMPLIMENT", "Compliment", 20));
    actCatalog.push_back(ActAction("JOKE", "Tell a joke", 18));
    actCatalog.push_back(ActAction("INSULT", "Insult", -10));
    actCatalog.push_back(ActAction("OFFER_SNACK", "Offer snack", 24));
    actCatalog.push_back(ActAction("DISCUSS", "Discuss", 14));
    actCatalog.push_back(ActAction("RESPECT", "Show respect", 22));
    actCatalog.push_back(ActAction("APOLOGIZE", "Apologize", 25));
    actCatalog.push_back(ActAction("REASON", "Reason calmly", 20));
    actCatalog.push_back(ActAction("CHALLENGE", "Challenge honorably", 8));
    actCatalog.push_back(ActAction("DANCE", "Dance", 16));
    actCatalog.push_back(ActAction("TAUNT", "Taunt", -15));
    actCatalog.push_back(ActAction("PRAY", "Pray", 30));
}

Game::~Game()
{
    for (Monster* monster : monstersPool) {
        delete monster;
    }
}

void Game::loadFiles()
{
    loadItems("Data/items.csv");
    loadMonsters("Data/monsters.csv");
}

void Game::loadItems(string fileName)
{
    ifstream file(fileName);

    if (!file.is_open()) {
        throw runtime_error("Impossible d'ouvrir " + fileName);
    }

    player.clearItems();

    string line;
    getline(file, line);

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        vector<string> fields = splitCsvLine(line);
        if (fields.size() < 4) {
            continue;
        }

        Item item(fields[0], parseItemType(fields[1]), stoi(fields[2]));
        player.addItem(item, stoi(fields[3]));
    }
}

void Game::loadMonsters(string fileName)
{
    ifstream file(fileName);

    if (!file.is_open()) {
        throw runtime_error("Impossible d'ouvrir " + fileName);
    }

    for (Monster* monster : monstersPool) {
        delete monster;
    }
    monstersPool.clear();
    bestiary.clear();

    string line;
    getline(file, line);

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        vector<string> fields = splitCsvLine(line);
        if (fields.size() < 6) {
            continue;
        }

        MonsterType category = parseMonsterType(fields[0]);
        const string& name = fields[1];
        int hp = stoi(fields[2]);
        int atk = stoi(fields[3]);
        int def = stoi(fields[4]);
        int mercyGoal = stoi(fields[5]);
        vector<ActAction*> acts;

        for (int i = 6; i < (int)fields.size(); i++) {
            if (fields[i] == "-") {
                continue;
            }

            for (ActAction& act : actCatalog) {
                if (act.getId() == fields[i]) {
                    acts.push_back(&act);
                    break;
                }
            }
        }

        monstersPool.push_back(new Monster(name, hp, atk, def, mercyGoal, category, acts));
        bestiary.push_back(BestiaryEntry(name, category, hp, atk, def, false));
    }
}

void Game::showMenu()
{
    bool running = true;

    while (running) {
        renderer.showMainMenu();
        int choice = readInt();

        switch (choice) {
            case 1:
                startCombat();
                break;
            case 2:
                renderer.showBestiary(bestiary);
                renderer.waitForEnter();
                break;
            case 3:
                renderer.showInventory(player.getItems());
                renderer.waitForEnter();
                break;
            case 4:
                try {
                    loadFiles();
                    renderer.showMessage("CSV data reloaded.");
                } catch (const exception& exception) {
                    renderer.showMessage(exception.what());
                }
                renderer.waitForEnter();
                break;
            case 5:
                running = false;
                break;
            default:
                renderer.showMessage("Invalid command.");
                break;
        }
    }
}

void Game::startCombat()
{
    if (monstersPool.empty()) {
        renderer.showMessage("No monsters available.");
        renderer.waitForEnter();
        return;
    }

    Monster* selectedMonster = selectRandomMonster();
    if (selectedMonster == nullptr) {
        renderer.showMessage("No monster selected.");
        renderer.waitForEnter();
        return;
    }

    bool sparedMonster = false;

    while (player.isAlive() && selectedMonster->isAlive()) {
        sparedMonster = false;
        renderer.showMonster(*selectedMonster);
        renderer.showPlayerStats(player);
        renderer.showCombatMenu();

        int choice = readInt();

        switch (choice) {
            case 1:
                handleFight(*selectedMonster);
                break;
            case 2:
                handleAct(*selectedMonster);
                break;
            case 3:
                handleItem();
                break;
            case 4:
                sparedMonster = selectedMonster->canBeMercied();
                handleMercy(*selectedMonster);
                break;
            default:
                renderer.showMessage("Invalid combat command.");
                continue;
        }

        if (!selectedMonster->isAlive()) {
            if (sparedMonster) {
                renderer.showMessage(selectedMonster->getName() + " was spared.");
            } else {
                player.addVictory();
                player.addKill();
                addBestiaryEntry(*selectedMonster, false);
                renderer.showMessage(selectedMonster->getName() + " was defeated.");
            }
            break;
        }

        if (selectedMonster->canBeMercied()) {
            renderer.showMessage(selectedMonster->getName() + " can now be spared.");
        }

        if (selectedMonster->isAlive()) {
            int damage = selectedMonster->attack(player);
            renderer.showMessage(selectedMonster->getName() + " attacks for " + to_string(damage) + " damage.");
        }
    }

    if (!player.isAlive()) {
        renderer.showMessage("You were defeated.");
    }

    delete selectedMonster;
    renderer.waitForEnter();
}

Monster* Game::selectRandomMonster()
{
    if (monstersPool.empty()) {
        return nullptr;
    }

    int index = rand() % monstersPool.size();
    return new Monster(*monstersPool[index]);
}

void Game::handleFight(Monster& monster)
{
    const int damage = max(1, 10 - monster.getDef());
    monster.takeDamage(damage);
    renderer.showMessage("You attack " + monster.getName() + " for " + to_string(damage) + " damage.");
}

void Game::handleAct(Monster& monster)
{
    vector<ActAction*> acts = monster.getActs();

    if (acts.empty()) {
        renderer.showMessage("This monster has no ACT options.");
        return;
    }

    for (int i = 0; i < (int)acts.size(); i++) {
        renderer.showMessage("[" + to_string(i + 1) + "] " + acts[i]->getDisplayText());
    }

    int choice = readInt();
    if (choice < 1 || choice > (int)acts.size()) {
        renderer.showMessage("Invalid ACT choice.");
        return;
    }

    monster.applyAct(*acts[choice - 1]);
    renderer.showMessage("You used " + acts[choice - 1]->getDisplayText() + ".");
}

void Game::handleItem()
{
    if (!player.hasItems()) {
        renderer.showMessage("Inventory empty.");
        return;
    }

    renderer.showInventory(player.getItems());
    int choice = readInt();

    if (choice < 1 || choice > (int)player.getItems().size()) {
        renderer.showMessage("Invalid item choice.");
        return;
    }

    string itemName = player.getItems()[choice - 1].getItem().getName();
    player.useItem(choice - 1);
    renderer.showMessage("You used " + itemName + ".");
}

void Game::handleMercy(Monster& monster)
{
    if (!monster.canBeMercied()) {
        renderer.showMessage("Mercy is not available yet.");
        return;
    }

    player.addVictory();
    player.addSpare();
    addBestiaryEntry(monster, true);
    monster.takeDamage(monster.getHp());
    renderer.showMessage("You spared " + monster.getName() + ".");
}

void Game::addBestiaryEntry(Monster monster, bool spared)
{
    for (BestiaryEntry& entry : bestiary) {
        if (entry.getMonsterName() == monster.getName()) {
            entry.setSpared(spared);
            return;
        }
    }

    bestiary.push_back(BestiaryEntry(
        monster.getName(),
        monster.getCategory(),
        monster.getMaxHp(),
        monster.getAtk(),
        monster.getDef(),
        spared
    ));
}

EndType Game::getEnding()
{
    if (!player.isAlive()) {
        return EndType::DEFEAT;
    }

    if (player.getVictories() > 0 && player.getVictories() == player.getKills()) {
        return EndType::GENOCIDE;
    }

    if (player.getVictories() > 0 && player.getVictories() == player.getSpares()) {
        return EndType::PACIFIST;
    }

    return EndType::NEUTRAL;
}

void Game::run()
{
    try {
        loadFiles();
        renderer.showMessage("Files loaded successfully.");
        showMenu();
    } catch (const exception& exception) {
        renderer.showMessage(exception.what());
    }
}
