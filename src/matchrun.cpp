#include "matchrun.h"


/* 
    Players array 0 and 1 are BLUE team, 2 and 3 are RED
    Screen is the screen to display score and text
*/

MatchRun::MatchRun(Screen * screen)
{
    // __matchdata.playerBlueA = players[0];
    // __matchdata.playerBlueB = players[1];
    // __matchdata.playerRedA = players[2];
    // __matchdata.playerRedB = players[3];
    __is_finished = false;
    __matchdata.blue_score = 0;
    __matchdata.red_score = 0;
    __screen = screen;
    // __storage = storage;
    screen -> displaytext("GO!!!", 4);
    delay(3000);
    screen -> clear();
    screen -> show_blue_score(__matchdata.blue_score);
    screen -> show_red_score(__matchdata.red_score);
};

// Interrupt driven handler, debounces ball detections
void MatchRun::blue_score_handle()
{
    int current_time = millis();
    int treshold = current_time - __last_goal_time;

    if (treshold >= 3000)
    {
        __last_goal_time = current_time;
        __matchdata.blue_score = __matchdata.blue_score + 1;
        if (__matchdata.blue_score >= 10)
        {
            __screen->displaytext("Blue team wins!", 2);
            __is_finished = true;
            
        }
        else if (!__is_finished)
        {
            __screen->show_blue_score(__matchdata.blue_score);
        }
    };
};

void MatchRun::red_score_handle()
{
    int current_time = millis();
    int treshold = current_time - __last_goal_time;
    if (treshold >= 3000)
    {
        __last_goal_time = current_time;
        __matchdata.red_score = __matchdata.red_score + 1;
        if (__matchdata.red_score >= 10)
        {
            __screen->displaytext("Red team wins!", 2);
            __is_finished = true;
            
        }
        else if (!__is_finished)
        {
            __screen->show_red_score(__matchdata.red_score);
        }
    };
};

bool MatchRun::is_finished()
{
    return __is_finished;
};

MatchRun::~MatchRun()
{
    __screen -> displaytext("Saving match results.", 2);
    delay(300);
    __screen -> displaytext("Saving match results..", 2);
    delay(300);
    __screen -> displaytext("Saving match results...", 2);
    return;
};