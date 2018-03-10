#ifndef MATCHTYPES
#define MATCHTYPES 

#define TEAMRED 0
#define TEAMBLUE 1

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
  team win;
  matchtime time;
};
#endif