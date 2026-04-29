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
    MonsterType category;
    vector<ActAction*> acts;
public:
    Monster(string name, int hp, int atk, int def, int mercyGoal, MonsterType category, vector<ActAction*> acts);
    virtual ~Monster() {}
    int attack(Entity& target) override;
    void applyAct(ActAction action);
    bool canBeMercied() const;
    int getMercy() const;
    int getMercyGoal() const;
    int getAtk() const;
    int getDef() const;
    vector<ActAction*> getActs() const;
    int getActCount() const;
    MonsterType getCategory() const;
    string getCategoryLabel() const;
};
#endif
