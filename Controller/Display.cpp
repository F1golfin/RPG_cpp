
#include "Display.h"
#include <iostream>


void Display::showMainMenu() 
{ 
    cout << "1. Fight | 2. Bestiary | 3. Exit" << endl; 
}

void Display::showCombatMenu() 
{ 
    cout << "A. Attack | B. Act | C. Item | D. Mercy" << endl; 
}

void Display::showPlayerStats(Player& p) 
{ 
    p.getStats(); 
}

void Display::showMonster(Monster& m) 
{ 
    cout << m.getName() << " HP:" << m.getHp() << " Mercy:" << m.getMercy() << endl; 
}

void Display::showInventory(Inventory& inv) 
{
    
    vector<InventorySlot> slots = inv.getItems();

    for (int i = 0; i < (int)slots.size(); i++) 
    {
        Item item = slots[i].getItem();
        int quantite = slots[i].getQuantity();
        
        cout << i << ". " << item.getName()  << " (Quantite: " << quantite << ")" << endl;
    }
}

void Display::showBestiary(vector<BestiaryEntry>& entries) 
{
    for (int i = 0; i < (int)entries.size(); i++) 
    {
        cout << i + 1 << ". "; 
        entries[i].display(); 
    }
}

void Display::showMessage(string t) 
{ 
    cout << t << endl; 
}
