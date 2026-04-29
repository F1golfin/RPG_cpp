
#include "ActAction.h"


ActAction::ActAction(string id, string displayText, int mercyImpact) 
    : id(id), displayText(displayText), mercyImpact(mercyImpact)
{
    // On stocke ici une action ACT du catalogue commun.
}


string ActAction::getId() const
{ 
    return id; 
}

string ActAction::getDisplayText() const
{ 
    return displayText;
}

int ActAction::getMercyImpact() const
{ 
    return mercyImpact; 
}
