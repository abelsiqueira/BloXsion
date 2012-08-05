#ifndef definitions_h
#define definitions_h

#include <string>

enum Scores { row3 = 3, row4 = 4, row5 = 5 };

const int cFps = 60;
const int cInitialObjects = 3;
const int cDifficulty = 10;
const int cMaxObjects = 14;
const float cFallingSpeed = 8;
const int cStartingTime = 10*cFps;
const int cSwapSpeed = 8;
const int cColorIntensity = 48;
const int cTimeReward[6]  = { 0, 0, 0, 2*cFps, 4*cFps, 7*cFps };
const int cScoreReward[6] = { 0, 0, 0, 10, 20, 30};

#endif
