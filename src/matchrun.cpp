#include "matchrun.h"


/* 
    Players array 0 and 1 are BLUE team, 2 and 3 are RED
    Screen is the screen to display score and text
*/

MatchRun::MatchRun(player players[4], Screen screen)
{
    return;
};

MatchRun::~MatchRun()
{
    return;
    //TODO close all the hardware things, write matchresult, send it to the backend, if not sent just store and report.
};