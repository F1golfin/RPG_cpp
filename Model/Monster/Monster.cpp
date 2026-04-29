
#include "Monster.h"
#include "../Player/Player.h"
#include <algorithm>

namespace {
int applyDefenseBonus(Entity& target, int damage)
{
    Player* player = dynamic_cast<Player*>(&target);

    if (player != nullptr) {
        // On reduit les degats recus grace aux items defensifs du joueur.
        damage = max(1, damage - player->getDefenseBonus());
    }

    target.takeDamage(damage);
    return damage;
}
}

Monster::Monster(string name, int hp, int atk, int def, int mercyGoal, MonsterType category, vector<ActAction*> acts, string introText)
    : Entity(name, hp), atk(atk), def(def), mercy(0), mercyGoal(mercyGoal), category(category), acts(acts), introText(introText)
{
    // On initialise un monstre avec ses statistiques et ses actions ACT.
}


int Monster::attack(Entity& target) 
{ 
    // Le sujet proposait des degats aleatoires entre 0 et les HP max.
    // On choisit des degats fixes avec ATK pour rendre les statistiques utiles.
    int damage = atk;
    return applyDefenseBonus(target, damage);
}

Monster* Monster::clone() const
{
    // On clone le monstre pour garder le type reel lors du combat.
    return new Monster(*this);
}

void Monster::applyAct(ActAction action) 
{ 
    mercy += action.getMercyImpact(); 
    // On garde la jauge Mercy entre 0 et l'objectif du monstre.
    mercy = max(0, min(mercy, mercyGoal));
}

void Monster::changeAttack(int amount)
{
    // On modifie l'attaque sans la laisser passer sous 1.
    atk = max(1, atk + amount);
}

void Monster::changeDefense(int amount)
{
    // On modifie la defense sans la laisser passer sous 0.
    def = max(0, def + amount);
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

string Monster::getIntroText() const
{
    return introText;
}

NormalMonster::NormalMonster(string name, int hp, int atk, int def, int mercyGoal, vector<ActAction*> acts, string introText)
    : Monster(name, hp, atk, def, mercyGoal, MonsterType::NORMAL, acts, introText)
{
    // On separe les monstres normaux pour rendre l'heritage visible dans le code.
}

int NormalMonster::attack(Entity& target)
{
    // On garde une attaque classique pour les ennemis de base.
    return Monster::attack(target);
}

Monster* NormalMonster::clone() const
{
    // On clone le type derive pour conserver le polymorphisme apres le tirage.
    return new NormalMonster(*this);
}

MiniBoss::MiniBoss(string name, int hp, int atk, int def, int mercyGoal, vector<ActAction*> acts, string introText)
    : Monster(name, hp, atk, def, mercyGoal, MonsterType::MINIBOSS, acts, introText)
{
    // On donne aux miniboss un comportement d'attaque specifique.
}

int MiniBoss::attack(Entity& target)
{
    int damage = atk;

    // On rend le miniboss plus dangereux tant que le joueur ne l'a pas calme avec ACT.
    if (mercy < mercyGoal / 2) {
        damage += 3;
    }

    return applyDefenseBonus(target, damage);
}

Monster* MiniBoss::clone() const
{
    // On clone le type derive pour conserver le polymorphisme apres le tirage.
    return new MiniBoss(*this);
}

BossMonster::BossMonster(string name, int hp, int atk, int def, int mercyGoal, vector<ActAction*> acts, string introText)
    : Monster(name, hp, atk, def, mercyGoal, MonsterType::BOSS, acts, introText), attackTurn(0)
{
    // On garde un compteur pour declencher une attaque speciale de boss.
}

int BossMonster::attack(Entity& target)
{
    attackTurn += 1;
    int damage = atk;

    // On redefinit l'attaque pour montrer un comportement propre aux boss.
    if (attackTurn % 3 == 0 || hp <= maxHp / 2) {
        damage += 6;
    }

    return applyDefenseBonus(target, damage);
}

Monster* BossMonster::clone() const
{
    // On clone le type derive pour conserver le polymorphisme apres le tirage.
    return new BossMonster(*this);
}
