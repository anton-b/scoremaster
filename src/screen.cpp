#include "screen.h"

Screen::Screen()
{
    __scorescreen = new Adafruit_SSD1306(OLED_RESET);
    __scorescreen->begin(SSD1306_SWITCHCAPVCC, 0x3C);
    clear();
    draw_header();
};

Screen::~Screen() 
{   
    __scorescreen->clearDisplay();
};

void Screen::show_blue_score(int score) 
{
    draw_header();
    drawscore(0, 20, 55, 40, score);
};

void Screen::show_red_score(int score) 
{
    draw_header();
    drawscore(77, 20, 55, 40, score);
};

void Screen::displaytext(char * text, int size)
{
    clear();
    __scorescreen->setCursor(0, 0);
    __scorescreen->setTextSize(size);
    __scorescreen->setTextColor(WHITE);
    __scorescreen-> print(text);
    show();
};

void Screen::drawscore(int x, int y, int w, int h, int score)
{
    __scorescreen->setTextSize(SCORESIZE);
    __scorescreen->setTextColor(WHITE);
    __scorescreen->setCursor(x, y);
    __scorescreen->fillRect(x, y, w, h, BLACK);
    __scorescreen->print(score);
    show();
};

void Screen::draw_header()
{
    __scorescreen->setTextSize(TEAMSIZE);
    __scorescreen->setTextColor(WHITE);
    __scorescreen->setCursor(0,0);
    __scorescreen->println(BLUETEAM);
    __scorescreen->setCursor(77,0);
    __scorescreen->println(REDTEAM);
    show();
};

void Screen::cleararea(int x, int y, int w, int h)
{
    __scorescreen -> fillRect(x, y, w, h, BLACK);
    show();
}

void Screen::clear()
{
    __scorescreen->clearDisplay();
};

void Screen::show()
{
    __scorescreen->display();
};