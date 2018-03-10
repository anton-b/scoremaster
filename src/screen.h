/*
    Class contains implementation of the scoreboard for 2 teams RED in BLUE with 2 fixed places for score(up to 10) 
*/

#ifndef SCREENLIB
#define SCREENLIB

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
#define BLUETEAM "Blue"
#define REDTEAM "Red"
#define TEAMSIZE 2
#define SCORESIZE 4

class Screen
{
    public:
        Screen();
        ~Screen();
        void show_red_score(int);
        void show_blue_score(int);
        void displaytext(char * text);
    private:
        Adafruit_SSD1306 * __scorescreen;
        void clear();
        void cleararea(int x, int y, int w, int h);
        void draw_header();
        void drawscore(int x, int y, int w, int h, int score);
        void show();
};
#endif