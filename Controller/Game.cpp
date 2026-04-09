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
                renderer.showBestiary(bestiary);
                renderer.waitForEnter();
                break;
            case 2:
                renderer.showInventory(player.getItems());
                renderer.waitForEnter();
                break;
            case 3:
                try {
                    loadFiles();
                    renderer.showMessage("CSV data reloaded.");
                } catch (const exception& exception) {
                    renderer.showMessage(exception.what());
                }
                renderer.waitForEnter();
                break;
            case 4:
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
    // TODO: Implement combat loop.
    renderer.showMessage("TODO: combat system not implemented yet.");
}

Monster* Game::selectRandomMonster()
{
    // TODO: Return a random monster from the pool when combat is implemented.
    return nullptr;
}

void Game::handleFight(Monster& monster)
{
    // TODO: Implement fight action.
    (void)monster;
}

void Game::handleAct(Monster& monster)
{
    // TODO: Implement ACT actions.
    (void)monster;
}

void Game::handleItem()
{
    // TODO: Implement item usage during combat.
}

void Game::handleMercy(Monster& monster)
{
    // TODO: Implement mercy flow.
    (void)monster;
}

void Game::addBestiaryEntry(Monster monster, bool spared)
{
    // TODO: Update bestiary state after a combat encounter.
    (void)monster;
    (void)spared;
}

EndType Game::getEnding()
{
    // TODO: Compute the ending once combat and progression are implemented.
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
