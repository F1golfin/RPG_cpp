
#include "ActAction.h"


ActAction::ActAction(string id, string displayText, int mercyImpact) 
    : id(id), displayText(displayText), mercyImpact(mercyImpact)
{
}


string ActAction::getId() 
{ 
    return id; 
}

string ActAction::getDisplayText() 
{ 
    return displayText;
}

int ActAction::getMercyImpact()  
{ 
    return mercyImpact; 
}
