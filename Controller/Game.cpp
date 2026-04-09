
#include "Game.h"
#include <iostream>


Game::Game() : player("Frisk", 20) 
{

}


void Game::run() 
{ 
    renderer.showMessage("Welcome to the RPG!"); 
    showMenu(); 
}

void Game::showMenu() 
{ 
    renderer.showMainMenu(); 
}