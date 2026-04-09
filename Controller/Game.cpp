
#include "Game.h"
#include <iostream>
#include <fstream>
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
}

Game::Game() : player("Frisk", 20), filesLoaded(false)
{
    loadActCatalog();
}

Game::~Game()
{
    for (Monster* monster : monstersPool) {
        delete monster;
    }
}

void Game::loadActCatalog()
{
    actCatalog.clear();
    actCatalog.reserve(13);

    actCatalog.emplace_back("OBSERVE", "Observe", 15);
    actCatalog.emplace_back("COMPLIMENT", "Compliment", 20);
    actCatalog.emplace_back("JOKE", "Tell a joke", 18);
    actCatalog.emplace_back("INSULT", "Insult", -10);
    actCatalog.emplace_back("OFFER_SNACK", "Offer snack", 24);
    actCatalog.emplace_back("DISCUSS", "Discuss", 14);
    actCatalog.emplace_back("RESPECT", "Show respect", 22);
    actCatalog.emplace_back("APOLOGIZE", "Apologize", 25);
    actCatalog.emplace_back("REASON", "Reason calmly", 20);
    actCatalog.emplace_back("CHALLENGE", "Challenge honorably", 8);
    actCatalog.emplace_back("DANCE", "Dance", 16);
    actCatalog.emplace_back("TAUNT", "Taunt", -15);
    actCatalog.emplace_back("PRAY", "Pray", 30);
}

void Game::loadMonstersCsv(const string& filePath)
{
    ifstream file(filePath);

    if (!file.is_open()) {
        throw runtime_error("Impossible d'ouvrir " + filePath);
    }

    string line;
    getline(file, line);

    for (Monster* monster : monstersPool) {
        delete monster;
    }
    monstersPool.clear();
    bestiary.clear();

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

        Monster* monster = nullptr;

        if (category == MonsterType::NORMAL) {
            monster = new NormalMonster(name, hp, atk, def, mercyGoal, acts);
        } else if (category == MonsterType::MINIBOSS) {
            monster = new MiniBoss(name, hp, atk, def, mercyGoal, acts);
        } else {
            monster = new Boss(name, hp, atk, def, mercyGoal, acts);
        }

        monstersPool.push_back(monster);
        bestiary.emplace_back(name, category, hp, atk, def, false);
    }
}

void Game::loadItemsCsv(const string& filePath)
{
    ifstream file(filePath);

    if (!file.is_open()) {
        throw runtime_error("Impossible d'ouvrir " + filePath);
    }

    player.getInventory() = Inventory();

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
        int quantity = stoi(fields[3]);
        player.getInventory().addItem(item, quantity);
    }
}

void Game::loadFiles()
{
    loadMonstersCsv("Data/monsters.csv");
    loadItemsCsv("Data/items.csv");
    filesLoaded = true;
}

void Game::run() 
{ 
    try {
        loadFiles();
        renderer.showHeader("RPG BOOT", "CSV assets loaded successfully");
        renderer.showMessage("Bestiary and inventory are ready.");
    } catch (const exception& exception) {
        renderer.showHeader("RPG BOOT", "CSV loading failed");
        renderer.showMessage(exception.what());
    }

    showMenu(); 
}

void Game::showMenu() 
{ 
    bool running = true;

    while (running) {
        renderer.showMainMenu();

        int choice = 0;
        cin >> choice;

        if (cin.fail()) {
            if (cin.eof()) {
                break;
            }

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            renderer.showMessage("Invalid input. Enter a number from 1 to 4.");
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                renderer.showBestiary(bestiary);
                renderer.waitForEnter();
                break;
            case 2:
                renderer.showInventory(player.getInventory());
                renderer.waitForEnter();
                break;
            case 3:
                try {
                    loadFiles();
                    renderer.showHeader("RELOAD", "CSV files reloaded");
                    renderer.showMessage("All data has been refreshed from Data/.");
                } catch (const exception& exception) {
                    renderer.showHeader("RELOAD", "Reload failed");
                    renderer.showMessage(exception.what());
                }
                renderer.waitForEnter();
                break;
            case 4:
                running = false;
                break;
            default:
                renderer.showMessage("Unknown command. Choose 1, 2, 3 or 4.");
                break;
        }
    }

    renderer.showHeader("SESSION CLOSED", filesLoaded ? "See you in the next run." : "Shutdown with missing data.");
}
