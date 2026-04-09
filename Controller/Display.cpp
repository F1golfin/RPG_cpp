
#include "Display.h"
#include <iostream>
#include <limits>

namespace {
string repeat(char value, int count)
{
    return string(count, value);
}
}

void Display::showHeader(const string& title, const string& subtitle)
{
    cout << repeat('=', 68) << endl;
    cout << "| " << title << endl;

    if (!subtitle.empty()) {
        cout << "| " << subtitle << endl;
    }

    cout << repeat('=', 68) << endl;
}

void Display::showSectionTitle(const string& title)
{
    cout << endl;
    cout << "+-- " << title << " " << repeat('-', max(1, 62 - (int)title.size())) << endl;
}

void Display::showMainMenu() 
{ 
    showHeader("The Tower of eprajelle", "Console bestiary loaded from CSV");
    cout << "| [1] View bestiary" << endl;
    cout << "| [2] View inventory" << endl;
    cout << "| [3] Reload CSV data" << endl;
    cout << "| [4] Exit" << endl;
    cout << repeat('=', 68) << endl;
    cout << "Select an action > ";
}

void Display::showCombatMenu() 
{ 
    cout << "A. Attack | B. Act | C. Item | D. Mercy" << endl; 
}

void Display::showPlayerStats(Player& p) 
{ 
    showSectionTitle("PLAYER STATUS");
    p.getStats(); 
}

void Display::showMonster(Monster& m) 
{ 
    cout << "| " << m.getName() << " HP:" << m.getHp() << "/" << m.getMaxHp()
         << " ATK:" << m.getAtk() << " DEF:" << m.getDef()
         << " MERCY:" << m.getMercy() << "/" << m.getMercyGoal() << endl;
}

void Display::showInventory(Inventory& inv) 
{
    showHeader("PLAYER INVENTORY", "Imported from Data/items.csv");
    vector<InventorySlot> slots = inv.getItems();

    if (slots.empty()) {
        cout << "| Inventory empty." << endl;
        cout << repeat('=', 68) << endl;
        return;
    }

    for (int i = 0; i < (int)slots.size(); i++) 
    {
        Item item = slots[i].getItem();
        int quantite = slots[i].getQuantity();
        
        cout << "| [" << i + 1 << "] " << item.getName()
             << " | Qty:" << quantite
             << " | Value:" << item.getValue() << endl;
    }

    cout << repeat('=', 68) << endl;
}

void Display::showBestiary(const vector<BestiaryEntry>& entries) 
{
    showHeader("BESTIARY ARCHIVE", "Imported from Data/monsters.csv");

    if (entries.empty()) {
        cout << "| No monsters loaded." << endl;
        cout << repeat('=', 68) << endl;
        return;
    }

    for (int i = 0; i < (int)entries.size(); i++) 
    {
        cout << "| [" << i + 1 << "] "; 
        entries[i].display(); 
    }

    cout << repeat('=', 68) << endl;
}

void Display::showMessage(string t) 
{ 
    cout << "| " << t << endl; 
}

void Display::waitForEnter()
{
    cout << endl << "Press ENTER to return to the menu...";
    char character = '\0';

    while (cin.get(character)) {
        if (character == '\n') {
            break;
        }
    }
}
