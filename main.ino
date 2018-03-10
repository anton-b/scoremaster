#include <Arduino.h>
// #include "src/types.h"
// #include "src/storage.h"
#include "src/screen.h"
#include "src/matchrun.h"
#define I 5 //25 worked, 19 worked, 18 worked

Screen * s;
volatile int bluescore = 0;
volatile int prevgoaltime = 0;


void setup()
{
    prevgoaltime = millis();
    Serial.begin(115200);
    s = new Screen();
    pinMode(I, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(I), bluescored, FALLING);
};

void loop()
{
    Serial.print(digitalRead(I));
    delay(1000);
    // x++;
    // if (x == 10) {x = 0;};
    // s -> show_red_score(x);
    // s -> show_blue_score(x);
    // delay(1000);
    // s -> show_blue_score(10);
};

void bluescored()
{
    int currenttime = millis();
    int treshold = currenttime - prevgoaltime;
    if (treshold >= 3000)
    {
        prevgoaltime = currenttime;
        bluescore++;
        s -> show_blue_score(bluescore);
    };
}