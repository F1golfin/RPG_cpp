
#include "ActAction.h"


ActAction::ActAction(string id, string displayText, int mercyImpact) 
    : id(id), displayText(displayText), mercyImpact(mercyImpact)
{
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
