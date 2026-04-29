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
const int VICTORY_GOAL = 10;

vector<string> splitCsvLine(const string &line) {
  // On decoupe une ligne CSV en utilisant le separateur choisi dans les fichiers.
  vector<string> tokens;
  string token;
  stringstream stream(line);

  while (getline(stream, token, ';')) {
    tokens.push_back(token);
  }

  return tokens;
}

MonsterType parseMonsterType(const string &value) {
  // On convertit le texte du CSV en enum MonsterType.
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

ItemType parseItemType(const string &value) {
  // On convertit le texte du CSV en enum ItemType.
  if (value == "HEAL") {
    return ItemType::HEAL;
  }

  throw runtime_error("Unknown item type: " + value);
}

int readInt() {
  // On lit un entier et on renvoie -1 si l'utilisateur tape autre chose.
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
} // namespace

void Game::askPlayerName() {
  string playerName;

  // On demande le nom au lancement, comme demande dans le sujet.
  cout << "Enter your character name > ";
  getline(cin, playerName);

  if (!playerName.empty()) {
    player.setName(playerName);
  }
}

Game::Game() : player("Frisk", 20) {
  srand((unsigned int)time(nullptr));

  // On cree le catalogue commun : le CSV indique seulement les identifiants.
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

Game::~Game() {
  // On libere les monstres crees avec new dans le pool.
  for (Monster *monster : monstersPool) {
    delete monster;
  }
}

void Game::loadFiles() {
  // On charge les deux fichiers obligatoires du projet.
  loadItems("Data/items.csv");
  loadMonsters("Data/monsters.csv");
}

void Game::loadItems(string fileName) {
  // On charge l'inventaire initial depuis items.csv.
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
      // On ignore les lignes mal formees pour eviter de faire planter le jeu.
      continue;
    }

    Item item(fields[0], parseItemType(fields[1]), stoi(fields[2]));
    player.addItem(item, stoi(fields[3]));
  }
}

void Game::loadMonsters(string fileName) {
  // On charge tous les monstres possibles depuis monsters.csv.
  ifstream file(fileName);

  if (!file.is_open()) {
    throw runtime_error("Impossible d'ouvrir " + fileName);
  }

  for (Monster *monster : monstersPool) {
    delete monster;
  }
  monstersPool.clear();

  // On separe le bestiaire complet et l'historique des combats gagnes.
  completeBestiary.clear();
  combatHistory.clear();

  string line;
  getline(file, line);

  while (getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    vector<string> fields = splitCsvLine(line);
    if (fields.size() < 6) {
      // On ignore aussi les lignes de monstres incompletes.
      continue;
    }

    MonsterType category = parseMonsterType(fields[0]);
    const string &name = fields[1];
    int hp = stoi(fields[2]);
    int atk = stoi(fields[3]);
    int def = stoi(fields[4]);
    int mercyGoal = stoi(fields[5]);
    vector<ActAction *> acts;

    for (int i = 6; i < (int)fields.size(); i++) {
      if (fields[i] == "-") {
        // On saute les cases vides utilisees pour les monstres avec moins d'ACT.
        continue;
      }

      for (ActAction &act : actCatalog) {
        if (act.getId() == fields[i]) {
          // On relie l'identifiant du CSV a l'action du catalogue.
          acts.push_back(&act);
          break;
        }
      }
    }

    monstersPool.push_back(
        new Monster(name, hp, atk, def, mercyGoal, category, acts));
    completeBestiary.push_back(BestiaryEntry(name, category, hp, atk, def,
                                             EncounterStatus::NOT_FOUGHT));
  }
}

void Game::showMenu() {
  // On garde le menu actif tant que le joueur ne quitte pas ou ne finit pas la partie.
  bool running = true;

  while (running) {
    renderer.showMainMenu();
    int choice = readInt();

    switch (choice) {
    case 1:
      // On lance un combat puis on verifie si la partie est terminee.
      startCombat();
      if (!player.isAlive() || player.getVictories() >= VICTORY_GOAL) {
        showEnding();
        running = false;
      }
      break;
    case 2:
      // On affiche tous les monstres connus du jeu.
      renderer.showBestiary(completeBestiary, "COMPLETE BESTIARY");
      renderer.waitForEnter();
      break;
    case 3:
      // On affiche seulement les monstres deja vaincus pendant la partie.
      renderer.showBestiary(combatHistory, "COMBAT HISTORY");
      renderer.waitForEnter();
      break;
    case 4:
      renderer.showPlayerStats(player);
      renderer.waitForEnter();
      break;
    case 5:
      // On autorise l'utilisation d'un item meme hors combat.
      renderer.showMessage("Choose an item number to use it, or 0 to cancel.");
      handleItem();
      renderer.waitForEnter();
      break;
    case 6:
      try {
        loadFiles();
        renderer.showMessage("CSV data reloaded.");
      } catch (const exception &exception) {
        renderer.showMessage(exception.what());
      }
      renderer.waitForEnter();
      break;
    case 7:
      running = false;
      break;
    default:
      renderer.showMessage("Invalid command.");
      break;
    }
  }
}

void Game::startCombat() {
  // On choisit un monstre et on joue des tours jusqu'a la fin du combat.
  if (monstersPool.empty()) {
    renderer.showMessage("No monsters available.");
    renderer.waitForEnter();
    return;
  }

  Monster *selectedMonster = selectRandomMonster();
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
      // FIGHT : on attaque directement le monstre.
      handleFight(*selectedMonster);
      break;
    case 2:
      // ACT : on modifie la jauge Mercy avec une action.
      handleAct(*selectedMonster);
      break;
    case 3:
      // ITEM : on utilise un objet et cela consomme le tour.
      handleItem();
      break;
    case 4:
      // MERCY : on epargne seulement si la jauge est suffisante.
      sparedMonster = selectedMonster->canBeMercied();
      handleMercy(*selectedMonster);
      break;
    default:
      renderer.showMessage("Invalid combat command.");
      continue;
    }

    if (!selectedMonster->isAlive()) {
      // On enregistre le resultat uniquement quand le combat est gagne.
      if (sparedMonster) {
        renderer.showMessage(selectedMonster->getName() + " was spared.");
      } else {
        player.addVictory();
        player.addKill();
        recordCombatResult(*selectedMonster, EncounterStatus::KILLED);
        renderer.showMessage(selectedMonster->getName() + " was defeated.");
      }
      break;
    }

    if (selectedMonster->canBeMercied()) {
      renderer.showMessage(selectedMonster->getName() + " can now be spared.");
    }

    if (selectedMonster->isAlive()) {
      int damage = selectedMonster->attack(player);
      renderer.showMessage(selectedMonster->getName() + " attacks for " +
                           to_string(damage) + " damage.");
    }
  }

  if (!player.isAlive()) {
    renderer.showMessage("You were defeated.");
  }

  delete selectedMonster;
  renderer.waitForEnter();
}

Monster *Game::selectRandomMonster() {
  // On tire seulement le monstre au hasard, pas les degats.
  if (monstersPool.empty()) {
    return nullptr;
  }

  int index = rand() % monstersPool.size();
  return new Monster(*monstersPool[index]);
}

void Game::handleFight(Monster &monster) {
  // Le sujet proposait des degats aleatoires entre 0 et les HP max.
  // On choisit des degats fixes, reduits par la defense du monstre.
  const int damage = max(1, 10 - monster.getDef());
  monster.takeDamage(damage);
  renderer.showMessage("You attack " + monster.getName() + " for " +
                       to_string(damage) + " damage.");
}

void Game::handleAct(Monster &monster) {
  // On affiche les actions ACT disponibles pour ce monstre.
  vector<ActAction *> acts = monster.getActs();

  if (acts.empty()) {
    renderer.showMessage("This monster has no ACT options.");
    return;
  }

  for (int i = 0; i < (int)acts.size(); i++) {
    renderer.showMessage("[" + to_string(i + 1) + "] " +
                         acts[i]->getDisplayText());
  }

  int choice = readInt();
  if (choice < 1 || choice > (int)acts.size()) {
    renderer.showMessage("Invalid ACT choice.");
    return;
  }

  monster.applyAct(*acts[choice - 1]);
  // On applique l'impact de l'action sur la jauge Mercy.
  renderer.showMessage("You used " + acts[choice - 1]->getDisplayText() + ".");
}

void Game::handleItem() {
  // On permet au joueur de choisir un item a consommer.
  if (!player.hasItems()) {
    renderer.showMessage("Inventory empty.");
    return;
  }

  renderer.showInventory(player.getItems());
  int choice = readInt();

  if (choice == 0) {
    renderer.showMessage("Cancelled.");
    return;
  }

  if (choice < 1 || choice > (int)player.getItems().size()) {
    renderer.showMessage("Invalid item choice.");
    return;
  }

  string itemName = player.getItems()[choice - 1].getItem().getName();
  player.useItem(choice - 1);
  renderer.showMessage("You used " + itemName + ".");
}

void Game::handleMercy(Monster &monster) {
  // On verifie que Mercy est disponible avant d'epargner.
  if (!monster.canBeMercied()) {
    renderer.showMessage("Mercy is not available yet.");
    return;
  }

  player.addVictory();
  player.addSpare();
  recordCombatResult(monster, EncounterStatus::SPARED);
  monster.takeDamage(monster.getHp());
  renderer.showMessage("You spared " + monster.getName() + ".");
}

void Game::recordCombatResult(const Monster &monster, EncounterStatus status) {
  // On met a jour le bestiaire complet et on ajoute une ligne a l'historique.
  for (BestiaryEntry &entry : completeBestiary) {
    if (entry.getMonsterName() == monster.getName()) {
      entry.setStatus(status);
      break;
    }
  }

  combatHistory.push_back(BestiaryEntry(
      monster.getName(), monster.getCategory(), monster.getMaxHp(),
      monster.getAtk(), monster.getDef(), status));
}

EndType Game::getEnding() {
  // On determine la fin selon les compteurs de kills et de spares.
  if (!player.isAlive()) {
    return EndType::DEFEAT;
  }

  if (player.getVictories() > 0 && player.getVictories() == player.getKills()) {
    return EndType::GENOCIDE;
  }

  if (player.getVictories() > 0 &&
      player.getVictories() == player.getSpares()) {
    return EndType::PACIFIST;
  }

  return EndType::NEUTRAL;
}

void Game::showEnding() {
  // On affiche la fin et les monstres concernes par cette route.
  EndType ending = getEnding();

  renderer.showMessage("");
  renderer.showMessage("========== ENDING ==========");

  if (ending == EndType::GENOCIDE) {
    renderer.showMessage(
        "Genocide ending: every victory was obtained by killing.");
    renderer.showBestiary(combatHistory, "KILLED MONSTERS");
  } else if (ending == EndType::PACIFIST) {
    renderer.showMessage(
        "Pacifist ending: every victory was obtained by sparing.");
    renderer.showBestiary(combatHistory, "SPARED MONSTERS");
  } else if (ending == EndType::DEFEAT) {
    renderer.showMessage("Defeat ending: your journey ends here.");
  } else {
    renderer.showMessage(
        "Neutral ending: you killed some monsters and spared others.");
    renderer.showBestiary(combatHistory, "COMBAT HISTORY");
  }

  renderer.showMessage("============================");
  renderer.waitForEnter();
}

void Game::run() {
  try {
    // On lance la partie : nom, fichiers, resume initial, puis menu.
    askPlayerName();
    loadFiles();
    renderer.showMessage("Files loaded successfully.");
    renderer.showPlayerStats(player);
    renderer.showInventory(player.getItems());
    renderer.waitForEnter();
    showMenu();
  } catch (const exception &exception) {
    renderer.showMessage(exception.what());
  }
}
