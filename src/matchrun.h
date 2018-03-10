#include "types.h"
#include "screen.h"
#include "storage.h"

/*
MatchRun class implements control over how match is going on:
 - counts the goals with the given hardware sensors(interrupts to be hooked externally), 
 - reports the score on the screens, 
 - when match is finished, frees resources up
 - reports to the given outputs
*/
class MatchRun
{
    public:
        MatchRun(Screen * screen);
        void blue_score_handle();
        void red_score_handle();
        bool is_finished();
        ~MatchRun();
    private:
        bool __is_finished;
        struct matchrecord __matchdata;
        Screen * __screen;
        Storage * __storage;
        int __last_goal_time;//it is unlikely that goals will happen simultaneously, so using one var for both ;-)
};