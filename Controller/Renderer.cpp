#include "Renderer.h"
#include <iomanip>
#include <iostream>
#include <limits>

namespace {
const int SCREEN_WIDTH = 78;

string repeat(char value, int count)
{
    // On reutilise cette fonction pour dessiner les bordures de l'interface.
    return string(count, value);
}

void showSectionTitle(const string& title)
{
    // On centralise les titres pour garder le meme style partout.
    cout << repeat('=', SCREEN_WIDTH) << endl;
    cout << "| " << title << endl;
    cout << repeat('-', SCREEN_WIDTH) << endl;
}

}

void Renderer::showMainMenu()
{
    // On affiche le menu principal hors combat.
    cout << endl;
    cout << repeat('=', SCREEN_WIDTH) << endl;
    cout << "| THE TOWER OF EPRAJELLE" << endl;
    cout << repeat('-', SCREEN_WIDTH) << endl;
    cout << "| [1] Start combat" << endl;
    cout << "| [2] Complete bestiary" << endl;
    cout << "| [3] Combat history" << endl;
    cout << "| [4] Player statistics" << endl;
    cout << "| [5] Inventory / use item" << endl;
    cout << "| [6] Reload CSV data" << endl;
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
    cout << "| Player : " << left << setw(24) << player.getName()
         << " HP " << right << setw(3) << player.getHp() << "/" << setw(3) << player.getMaxHp()
         << " | Wins " << setw(2) << player.getVictories()
         << " | Kills " << setw(2) << player.getKills()
         << " | Spares " << setw(2) << player.getSpares()
         << " |" << endl;
}

void Renderer::showMonster(const Monster& monster)
{
    // On affiche la fiche du monstre au debut de chaque tour.
    cout << endl;
    cout << repeat('=', SCREEN_WIDTH) << endl;
    cout << "| Enemy  : " << left << setw(24) << monster.getName()
         << " Type " << setw(8) << monster.getCategoryLabel()
         << " | HP " << right << setw(3) << monster.getHp() << "/" << setw(3) << monster.getMaxHp()
         << " |" << endl;
    cout << "| Stats  : ATK " << setw(3) << monster.getAtk()
         << " | DEF " << setw(3) << monster.getDef()
         << " | Mercy " << setw(3) << monster.getMercy() << "/" << setw(3) << monster.getMercyGoal()
         << setw(38) << " |" << endl;
    cout << repeat('=', SCREEN_WIDTH) << endl;
}

void Renderer::showInventory(const vector<InventorySlot>& items)
{
    // On affiche l'inventaire sous forme de tableau.
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
    cout << text << endl;
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
