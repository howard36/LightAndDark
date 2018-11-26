#pragma once

#include "../include/Macros.h"

class level {
    board b;
    int initState;
    int difficulty; // number of moves in optimal solution

  public:
    level();
    level(const state _s);
    void play() const;
    static level hardLevel(int maxX, int maxY, int numColors, int numBalls, int minDifficulty = 0);
};