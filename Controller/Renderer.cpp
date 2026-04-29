#include "Renderer.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>

namespace {
const int SCREEN_WIDTH = 64;

string repeat(char value, int count)
{
    // On reutilise cette fonction pour dessiner les bordures de l'interface.
    return string(count, value);
}

string centerText(const string& text)
{
    // On centre un titre court dans la largeur de la console.
    int padding = max(0, (SCREEN_WIDTH - (int)text.size()) / 2);
    return repeat(' ', padding) + text;
}

string progressBar(int value, int maximum, int size)
{
    // On transforme une valeur numerique en petite barre visuelle.
    if (maximum <= 0) {
        return repeat('-', size);
    }

    value = max(0, min(value, maximum));
    int filled = value * size / maximum;
    return repeat('#', filled) + repeat('-', size - filled);
}

void showSectionTitle(const string& title)
{
    // On centralise les titres pour garder le meme style partout.
    cout << repeat('=', SCREEN_WIDTH) << endl;
    cout << "| " << title << endl;
    cout << repeat('-', SCREEN_WIDTH) << endl;
}

}

void Renderer::clearScreen()
{
    // On efface la console pour afficher seulement l'ecran courant.
    cout << "\033[2J\033[H";
}

void Renderer::showMainMenu()
{
    // On affiche le menu principal hors combat.
    clearScreen();
    cout << endl;
    cout << repeat('=', SCREEN_WIDTH) << endl;
    cout << centerText("THE TOWER OF EPRAJELLE") << endl;
    cout << centerText("Console RPG") << endl;
    cout << repeat('-', SCREEN_WIDTH) << endl;
    cout << "| [1] Start combat          | [2] Complete bestiary" << endl;
    cout << "| [3] Combat history        | [4] Player statistics" << endl;
    cout << "| [5] Inventory / use item  | [6] Reload CSV data" << endl;
    cout << "| [7] Exit" << endl;
    cout << repeat('=', SCREEN_WIDTH) << endl;
    cout << "Select an action > ";
}

void Renderer::showCombatMenu()
{
    // On affiche les quatre choix classiques du combat.
    cout << repeat('-', SCREEN_WIDTH) << endl;
    cout << "| [1] Fight     [2] Act     [3] Item     [4] Mercy" << endl;
    cout << repeat('-', SCREEN_WIDTH) << endl;
    cout << "Choose > ";
}

void Renderer::showPlayerStats(const Player& player)
{
    // On regroupe les statistiques importantes du joueur sur une ligne.
    cout << repeat('-', SCREEN_WIDTH) << endl;
    cout << "Player : " << player.getName() << endl;
    cout << "HP     : [" << progressBar(player.getHp(), player.getMaxHp(), 16) << "] "
         << player.getHp() << "/" << player.getMaxHp() << endl;
    cout << "Wins   : " << player.getVictories()
         << "   Kills: " << player.getKills()
         << "   Spares: " << player.getSpares() << endl;
    cout << repeat('-', SCREEN_WIDTH) << endl;
}

void Renderer::showMonster(const Monster& monster)
{
    // On affiche la fiche du monstre au debut de chaque tour.
    clearScreen();
    cout << endl;
    cout << repeat('=', SCREEN_WIDTH) << endl;
    cout << "Enemy  : " << monster.getName()
         << " (" << monster.getCategoryLabel() << ")" << endl;
    cout << "Stats  : ATK " << monster.getAtk()
         << " | DEF " << monster.getDef() << endl;
    cout << "HP     : [" << progressBar(monster.getHp(), monster.getMaxHp(), 24) << "] "
         << monster.getHp() << "/" << monster.getMaxHp() << endl;
    cout << "Mercy  : [" << progressBar(monster.getMercy(), monster.getMercyGoal(), 24) << "] "
         << monster.getMercy() << "/" << monster.getMercyGoal() << endl;
    cout << repeat('=', SCREEN_WIDTH) << endl;
}

void Renderer::showInventory(const vector<InventorySlot>& items)
{
    // On affiche l'inventaire sous forme de tableau.
    clearScreen();
    showSectionTitle("PLAYER INVENTORY");

    if (items.empty()) {
        cout << "| Inventory empty." << endl;
        cout << repeat('=', SCREEN_WIDTH) << endl;
        return;
    }

    cout << "| #  | Item                         | Type | Heal | Quantity |" << endl;
    cout << repeat('-', SCREEN_WIDTH) << endl;

    for (int i = 0; i < (int)items.size(); i++) {
        cout << "| " << right << setw(2) << i + 1
             << " | " << left << setw(28) << items[i].getItem().getName()
             << " | HEAL"
             << " | " << right << setw(4) << items[i].getItem().getValue()
             << " | " << setw(8) << items[i].getQuantity()
             << " |" << endl;
    }

    cout << repeat('=', SCREEN_WIDTH) << endl;
}

void Renderer::showBestiary(const vector<BestiaryEntry>& entries, const string& title)
{
    // On reutilise cette fonction pour le bestiaire complet et l'historique.
    clearScreen();
    showSectionTitle(title);

    if (entries.empty()) {
        cout << "| No entries." << endl;
        cout << repeat('=', SCREEN_WIDTH) << endl;
        return;
    }

    cout << "| Name                         | Type     |  HP | ATK | DEF | Status          |" << endl;
    cout << repeat('-', SCREEN_WIDTH) << endl;

    for (const BestiaryEntry& entry : entries) {
        entry.display();
    }

    cout << repeat('=', SCREEN_WIDTH) << endl;
}

void Renderer::showMessage(const string& text)
{
    if (text.empty()) {
        cout << endl;
        return;
    }

    cout << ">> " << text << endl;
}

void Renderer::waitForEnter()
{
    cout << endl << "Press ENTER to continue...";
    char character = '\0';

    while (cin.get(character)) {
        if (character == '\n') {
            break;
        }
    }
}
