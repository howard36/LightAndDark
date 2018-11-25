#pragma once

#include "../include/Macros.h"

class level {
    board b;
    state initState;

  public:
    level(state _s);
    void play() const;
    static level hardLevel(int maxX, int maxY, int numColors, int numBalls);
};