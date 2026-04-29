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
  if (value == "BUFF_ATTACK") {
    return ItemType::BUFF_ATTACK;
  }
  if (value == "BUFF_DEFENSE") {
    return ItemType::BUFF_DEFENSE;
  }

  throw runtime_error("Unknown item type: " + value);
}

int readInt() {
  // On lit un entier et on renvoie -1 si l'utilisateur tape autre chose.
  int value = 0;
  cin >> value;

  if (cin.fail()) {
    if (cin.eof()) {
      throw runtime_error("Input closed.");
    }

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

Game::Game() : player("Frisk", 60) {
  srand((unsigned int)time(nullptr));

  // On donne 60 HP au joueur pour que les premiers combats soient jouables.
  // Le nom "Frisk" sert seulement de valeur par defaut si le joueur ne tape rien.

  // On cree le catalogue commun : le CSV indique seulement les identifiants.
  // Les nombres positifs augmentent Mercy, les nombres negatifs l'abaissent.
  // On a augmente les valeurs positives pour que la route pacifiste soit viable.
  actCatalog.push_back(ActAction("OBSERVE", "Observe", 25));
  actCatalog.push_back(ActAction("COMPLIMENT", "Compliment", 30));
  actCatalog.push_back(ActAction("JOKE", "Tell a joke", 25));
  actCatalog.push_back(ActAction("INSULT", "Insult", -10));
  actCatalog.push_back(ActAction("OFFER_SNACK", "Offer snack", 35));
  actCatalog.push_back(ActAction("DISCUSS", "Discuss", 25));
  actCatalog.push_back(ActAction("RESPECT", "Show respect", 30));
  actCatalog.push_back(ActAction("APOLOGIZE", "Apologize", 35));
  actCatalog.push_back(ActAction("REASON", "Reason calmly", 30));
  actCatalog.push_back(ActAction("CHALLENGE", "Challenge honorably", 20));
  actCatalog.push_back(ActAction("DANCE", "Dance", 25));
  actCatalog.push_back(ActAction("TAUNT", "Taunt", -15));
  actCatalog.push_back(ActAction("PRAY", "Pray", 40));
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
  // On charge la reserve d'items disponible depuis items.csv.
  ifstream file(fileName);

  if (!file.is_open()) {
    throw runtime_error("Impossible d'ouvrir " + fileName);
  }

  player.clearItems();
  itemPool.clear();

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
    addItemToPool(item, stoi(fields[3]));
  }

  giveStartingItems();
}

void Game::addItemToPool(Item item, int quantity) {
  if (quantity <= 0) {
    return;
  }

  // On garde les quantites du CSV comme stock total droppable.
  for (InventorySlot &slot : itemPool) {
    if (slot.getItem().getName() == item.getName()) {
      slot.addQuantity(quantity);
      return;
    }
  }

  itemPool.push_back(InventorySlot(item, quantity));
}

bool Game::grantItemFromPool(int index) {
  if (index < 0 || index >= (int)itemPool.size()) {
    return false;
  }

  // On transfere un seul exemplaire du stock vers l'inventaire du joueur.
  Item item = itemPool[index].getItem();
  player.addItem(item, 1);
  itemPool[index].removeOne();

  if (itemPool[index].isEmpty()) {
    itemPool.erase(itemPool.begin() + index);
  }

  return true;
}

void Game::giveStartingItems() {
  // On commence avec seulement deux items pour laisser la progression aux drops.
  int itemCount = min(2, (int)itemPool.size());

  for (int i = 0; i < itemCount; i++) {
    vector<int> starterCandidates;

    // On privilegie des petits soins au depart pour eviter un debut trop facile.
    for (int j = 0; j < (int)itemPool.size(); j++) {
      Item item = itemPool[j].getItem();
      if (item.getType() == ItemType::HEAL && item.getValue() <= 25) {
        starterCandidates.push_back(j);
      }
    }

    int index = starterCandidates.empty()
                    ? rand() % itemPool.size()
                    : starterCandidates[rand() % starterCandidates.size()];
    grantItemFromPool(index);
  }
}

void Game::tryDropItem(MonsterType category) {
  if (itemPool.empty()) {
    renderer.showMessage("No items remain in the drop pool.");
    return;
  }

  int dropChance = 35;
  if (category == MonsterType::MINIBOSS) {
    dropChance = 60;
  } else if (category == MonsterType::BOSS) {
    dropChance = 85;
  }

  // On augmente la chance de drop selon la rarete du monstre tue.
  if (rand() % 100 >= dropChance) {
    renderer.showMessage("No item dropped.");
    return;
  }

  vector<int> candidates;
  for (int i = 0; i < (int)itemPool.size(); i++) {
    Item item = itemPool[i].getItem();

    // On adapte les drops au type de monstre pour eviter de donner les gros buffs trop tot.
    if (category == MonsterType::NORMAL && item.getType() == ItemType::HEAL &&
        item.getValue() <= 25) {
      candidates.push_back(i);
    } else if (category == MonsterType::MINIBOSS &&
               (item.getType() == ItemType::HEAL || item.getValue() <= 3)) {
      candidates.push_back(i);
    } else if (category == MonsterType::BOSS) {
      candidates.push_back(i);
    }
  }

  if (candidates.empty()) {
    for (int i = 0; i < (int)itemPool.size(); i++) {
      candidates.push_back(i);
    }
  }

  int selectedIndex = candidates[rand() % candidates.size()];
  string itemName = itemPool[selectedIndex].getItem().getName();
  grantItemFromPool(selectedIndex);
  renderer.showMessage("Dropped item: " + itemName + ".");
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
    string introText = "";

    if (fields.size() > 10) {
      // On recupere une phrase d'introduction pour rendre les monstres plus vivants.
      introText = fields[10];
    }

    int actLimit = min(10, (int)fields.size());
    for (int i = 6; i < actLimit; i++) {
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

    if (category == MonsterType::MINIBOSS) {
      // On cree une classe derivee pour rendre le comportement des miniboss polymorphe.
      monstersPool.push_back(new MiniBoss(name, hp, atk, def, mercyGoal, acts,
                                          introText));
    } else if (category == MonsterType::BOSS) {
      // On cree une classe derivee pour rendre le comportement des boss polymorphe.
      monstersPool.push_back(new BossMonster(name, hp, atk, def, mercyGoal,
                                             acts, introText));
    } else {
      // On garde aussi une classe derivee pour les monstres normaux.
      monstersPool.push_back(new NormalMonster(name, hp, atk, def, mercyGoal,
                                               acts, introText));
    }
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
  bool introShown = false;

  while (player.isAlive() && selectedMonster->isAlive()) {
    sparedMonster = false;
    renderer.showMonster(*selectedMonster);
    if (!introShown && !selectedMonster->getIntroText().empty()) {
      renderer.showMessage(selectedMonster->getIntroText());
      introShown = true;
    }
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
        tryDropItem(selectedMonster->getCategory());
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
  // On adapte aussi la categorie a l'avancement pour eviter un boss au premier combat.
  if (monstersPool.empty()) {
    return nullptr;
  }

  MonsterType wantedCategory = MonsterType::NORMAL;

  // On cree une progression simple :
  // 0 a 3 victoires  -> monstres NORMAL
  // 4 a 6 victoires  -> MINIBOSS
  // 7 a 10 victoires -> BOSS
  if (player.getVictories() >= 7) {
    wantedCategory = MonsterType::BOSS;
  } else if (player.getVictories() >= 4) {
    wantedCategory = MonsterType::MINIBOSS;
  }

  vector<Monster *> possibleMonsters;

  // On filtre le pool pour garder seulement les monstres de la categorie voulue.
  for (Monster *monster : monstersPool) {
    if (monster->getCategory() == wantedCategory) {
      possibleMonsters.push_back(monster);
    }
  }

  if (possibleMonsters.empty()) {
    // On garde une securite si le CSV ne contient pas la categorie demandee.
    possibleMonsters = monstersPool;
  }

  // On clone le monstre choisi pour garder son vrai type derive pendant le combat.
  int index = rand() % possibleMonsters.size();
  return possibleMonsters[index]->clone();
}

void Game::handleFight(Monster &monster) {
  // On calcule l'attaque du joueur avec les buffs puis la defense du monstre.
  const int damage = max(1, 10 + player.getAttackBonus() - monster.getDef());
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

  // On garde la Mercy avant/apres pour afficher clairement l'effet de l'ACT.
  int mercyBefore = monster.getMercy();
  monster.applyAct(*acts[choice - 1]);
  int mercyAfter = monster.getMercy();
  int mercyChange = mercyAfter - mercyBefore;

  // On applique l'impact de l'action sur la jauge Mercy.
  renderer.showMessage("You used " + acts[choice - 1]->getDisplayText() + ".");

  if (mercyChange >= 0) {
    renderer.showMessage("Mercy +" + to_string(mercyChange) + " (" +
                         to_string(mercyAfter) + "/" +
                         to_string(monster.getMercyGoal()) + ").");
  } else {
    renderer.showMessage("Mercy " + to_string(mercyChange) + " (" +
                         to_string(mercyAfter) + "/" +
                         to_string(monster.getMercyGoal()) + ").");
  }

  string actId = acts[choice - 1]->getId();

  // On applique un effet secondaire pour rendre ACT plus strategique.
  if (actId == "OBSERVE" || actId == "REASON") {
    monster.changeAttack(-2);
    renderer.showMessage(monster.getName() + " loses focus. ATK -2.");
  } else if (actId == "OFFER_SNACK" || actId == "APOLOGIZE") {
    player.heal(5);
    renderer.showMessage("You recover 5 HP while calming the situation.");
  } else if (actId == "INSULT" || actId == "TAUNT") {
    monster.changeAttack(2);
    renderer.showMessage(monster.getName() + " becomes angrier. ATK +2.");
  } else if (actId == "RESPECT" || actId == "CHALLENGE") {
    monster.changeDefense(-1);
    renderer.showMessage(monster.getName() + " lowers its guard. DEF -1.");
  }
}

void Game::handleItem() {
  // On permet au joueur de choisir un item a consommer.
  if (!player.hasItems()) {
    renderer.showMessage("Inventory empty.");
    return;
  }

  renderer.showInventory(player.getItems());
  renderer.showMessage("Choose an item number to use it, or 0 to cancel.");
  int choice = readInt();

  if (choice == 0) {
    renderer.showMessage("Cancelled.");
    return;
  }

  if (choice < 1 || choice > (int)player.getItems().size()) {
    renderer.showMessage("Invalid item choice.");
    return;
  }

  Item item = player.getItems()[choice - 1].getItem();
  string itemName = item.getName();
  int hpBefore = player.getHp();
  int attackBefore = player.getAttackBonus();
  int defenseBefore = player.getDefenseBonus();

  player.useItem(choice - 1);
  renderer.showMessage("You used " + itemName + ".");

  if (item.getType() == ItemType::HEAL) {
    // On affiche le soin reel pour que le joueur comprenne l'effet de l'item.
    int healed = player.getHp() - hpBefore;
    renderer.showMessage("HP +" + to_string(healed) + " (" +
                         to_string(player.getHp()) + "/" +
                         to_string(player.getMaxHp()) + ").");
  } else if (item.getType() == ItemType::BUFF_ATTACK) {
    // On affiche le bonus gagne pour que le joueur comprenne l'effet offensif.
    int gained = player.getAttackBonus() - attackBefore;
    renderer.showMessage("ATK bonus +" + to_string(gained) +
                         " (total +" + to_string(player.getAttackBonus()) +
                         ").");
  } else if (item.getType() == ItemType::BUFF_DEFENSE) {
    // On affiche le bonus gagne pour que le joueur comprenne l'effet defensif.
    int gained = player.getDefenseBonus() - defenseBefore;
    renderer.showMessage("DEF bonus +" + to_string(gained) +
                         " (total +" + to_string(player.getDefenseBonus()) +
                         ").");
  }
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

  if (player.getVictories() >= VICTORY_GOAL &&
      player.getVictories() == player.getSpares()) {
    // On distingue la route parfaite de la route pacifiste classique.
    return EndType::TRUE_PACIFIST;
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

  renderer.clearScreen();
  renderer.showMessage("");
  renderer.showMessage("========== ENDING ==========");

  renderer.showMessage("Route summary:");
  renderer.showMessage("Victories: " + to_string(player.getVictories()));
  renderer.showMessage("Kills: " + to_string(player.getKills()));
  renderer.showMessage("Spares: " + to_string(player.getSpares()));
  renderer.showMessage("");

  if (ending == EndType::GENOCIDE) {
    renderer.showMessage(
        "Genocide ending: every victory was obtained by killing.");
    renderer.showBestiary(combatHistory, "KILLED MONSTERS", false);
  } else if (ending == EndType::TRUE_PACIFIST) {
    renderer.showMessage(
        "True Pacifist ending: every monster was spared until the final victory.");
    renderer.showBestiary(combatHistory, "SPARED MONSTERS", false);
  } else if (ending == EndType::PACIFIST) {
    renderer.showMessage(
        "Pacifist ending: every victory was obtained by sparing.");
    renderer.showBestiary(combatHistory, "SPARED MONSTERS", false);
  } else if (ending == EndType::DEFEAT) {
    renderer.showMessage("Defeat ending: your journey ends here.");
  } else {
    renderer.showMessage(
        "Neutral ending: you killed some monsters and spared others.");
    renderer.showBestiary(combatHistory, "COMBAT HISTORY", false);
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
