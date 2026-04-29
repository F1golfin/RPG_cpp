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
    string introText;
public:
    Monster(string name, int hp, int atk, int def, int mercyGoal, MonsterType category, vector<ActAction*> acts, string introText = "");
    virtual ~Monster() {}
    int attack(Entity& target) override;
    virtual Monster* clone() const;
    void applyAct(ActAction action);
    void changeAttack(int amount);
    void changeDefense(int amount);
    bool canBeMercied() const;
    int getMercy() const;
    int getMercyGoal() const;
    int getAtk() const;
    int getDef() const;
    vector<ActAction*> getActs() const;
    int getActCount() const;
    MonsterType getCategory() const;
    string getCategoryLabel() const;
    string getIntroText() const;
};

class NormalMonster : public Monster {
public:
    NormalMonster(string name, int hp, int atk, int def, int mercyGoal, vector<ActAction*> acts, string introText = "");
    int attack(Entity& target) override;
    Monster* clone() const override;
};

class MiniBoss : public Monster {
public:
    MiniBoss(string name, int hp, int atk, int def, int mercyGoal, vector<ActAction*> acts, string introText = "");
    int attack(Entity& target) override;
    Monster* clone() const override;
};

class BossMonster : public Monster {
private:
    int attackTurn;
public:
    BossMonster(string name, int hp, int atk, int def, int mercyGoal, vector<ActAction*> acts, string introText = "");
    int attack(Entity& target) override;
    Monster* clone() const override;
};
#endif
