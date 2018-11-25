#pragma once

#include <bits/stdc++.h>
#define pi pair<int, int>
#define x first
#define y second
using namespace std;

#include "Board.h"

class state {
    vector<pi> ballPos;		// position of each ball
    vector<bool> colorFlip; // whether each color has been flipped from initial shade or not
    int hash;
    bool win; // whether the state is a winning state (all balls in target)
  public:
    const board b;

    state::state(board _b, vector<pi> _ballPos, vector<bool> _colorFlip);
    vector<pi> getBallPos() const { return ballPos; }
    vector<bool> getColorFlip() const { return colorFlip; }
    int getHash() const { return hash; }
    bool getWin() const { return win; }

    bool state::getShade(pi p) const;
    bool state::occupied(pi p) const;
    state move(int ball, int dir) const;
    void flip(int color);
    void moveBall(int ball, pi pos);
};