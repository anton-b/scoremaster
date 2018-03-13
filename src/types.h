#ifndef MATCHTYPES
#define MATCHTYPES 

#define TEAMRED 1
#define TEAMBLUE 0

typedef unsigned char uint8;
typedef bool team;
typedef int matchtime; //Match time in seconds

struct player
{
    uint8 id = 0;
    const char * name = "default";
};

/*
matchrecord the structure to be stored as binary in some arbitrary storage
*/
struct matchrecord
{
  uint8 id;
  player playerBlueA;
  player playerBlueB;
  player playerRedA;
  player playerRedB;
  int blue_score = 0;
  int red_score = 0;
  team win = TEAMBLUE;
  int time = 0;
  int timestart = 0; 
};
#endif