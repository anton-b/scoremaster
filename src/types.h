#ifndef MATCHTYPES
#define MATCHTYPES 

#define TEAMRED 1
#define TEAMBLUE 0

typedef unsigned char uint8;
typedef bool team;
typedef int matchtime; //Match time in seconds

struct player
{
    uint8 id;
    const char * name;
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
  int blue_score;
  int red_score;
  team win;
  int time;
};
#endif