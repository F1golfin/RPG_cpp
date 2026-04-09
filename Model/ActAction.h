#ifndef ACTACTION_H
#define ACTACTION_H
#include <string>
using namespace std;

class ActAction {
private:
    string id;
    string displayText;
    int mercyImpact;
public:
    ActAction(string id, string displayText, int mercyImpact);
    string getId();
    string getDisplayText();
    int getMercyImpact();
};
#endif