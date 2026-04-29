
#include "Monster.h"
#include <algorithm>


Monster::Monster(string name, int hp, int atk, int def, int mercyGoal, MonsterType category, vector<ActAction*> acts)
    : Entity(name, hp), atk(atk), def(def), mercy(0), mercyGoal(mercyGoal), category(category), acts(acts)
{
    // On initialise un monstre avec ses statistiques et ses actions ACT.
}


int Monster::attack(Entity& target) 
{ 
    // Le sujet proposait des degats aleatoires entre 0 et les HP max.
    // On choisit des degats fixes avec ATK pour rendre les statistiques utiles.
    int damage = atk;
    target.takeDamage(damage); 
    return damage; 
}

void Monster::applyAct(ActAction action) 
{ 
    mercy += action.getMercyImpact(); 
    // On garde la jauge Mercy entre 0 et l'objectif du monstre.
    mercy = max(0, min(mercy, mercyGoal));
}

bool Monster::canBeMercied() const
{ 
    // On peut epargner le monstre quand sa jauge Mercy atteint l'objectif.
    return mercy >= mercyGoal; 
}

int Monster::getMercy() const
{ 
    return mercy; 
}

int Monster::getMercyGoal() const
{ 
    return mercyGoal; 
}

int Monster::getAtk() const
{ 
    return atk; 
}

int Monster::getDef() const
{ 
    return def; 
}

vector<ActAction*> Monster::getActs() const
{ 
    // On renvoie les actions ACT disponibles pour ce monstre.
    return acts; 
}

int Monster::getActCount() const
{
    return (int)acts.size();
}

MonsterType Monster::getCategory() const
{
    return category;
}

string Monster::getCategoryLabel() const
{
    // On transforme l'enum en texte pour l'affichage console.
    if (category == MonsterType::MINIBOSS) {
        return "MINIBOSS";
    }

    if (category == MonsterType::BOSS) {
        return "BOSS";
    }

    return "NORMAL";
}
