#include "types.h"
#include "screen.h"

/*
MatchRun class implements control over how match is going on:
 - counts the goals with the given hardware sensors, 
 - reports the score on the screens, 
 - when match is finished, frees resources up
 - reports to the given outputs
*/
class MatchRun
{
    public:
        MatchRun(player players[4], Screen screen);
        ~MatchRun();
};