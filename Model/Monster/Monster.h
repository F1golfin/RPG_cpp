#ifndef MONSTER_H
#define MONSTER_H
#include "../Entity.h"
#include "../ActAction.h"
#include "../../Enums.h"
#include <vector>
using namespace std;

class Monster : public Entity {
protected:
    int atk; 
    int def;
    int mercy;
    int mercyGoal;
    vector<ActAction*> acts;
public:
    Monster(string name, int hp, int atk, int def, int mercyGoal, vector<ActAction*> acts);
    virtual int attack(Entity& target);
    void applyAct(ActAction action);
    bool canBeMercied();
    int getMercy();
    int getMercyGoal();
    int getAtk();
    int getDef();
    vector<ActAction*> getActs();
    virtual int getActCount() const = 0;  // A remodifier dans les sous classes. (avec le const = 0 et virtual)
    virtual MonsterType getCategory() const = 0;
};
#endif