#pragma once

#include <bits/stdc++.h>
#define pi pair<int, int>
#define ll long long
#define x first
#define y second
using namespace std;

#include "../include/State.h"

class board {
    vector<vector<int>> adj;
    vector<bool> validStates;
    vector<vector<ll>> paths; // paths[i][j] = number of length i (not necessarily simple) paths from state j to winning state
    vector<int> distance;
    int maxDist;
    int hardestState;

  public:
    const int maxX, maxY, numColors, numBalls;
    const vector<vector<bool>> grid;              // maxX x maxY grid of bools representing which squares are part of the board
    const vector<vector<vector<bool>>> colorGrid; // colorGrid[c] is a 2d grid representing which squares have color c
    const vector<vector<bool>> initShade;         // false = dark, true = light (initial values)
    const vector<vector<int>> button;             // button[x][y] = color of button at (x, y), or -1 if no button
    const pi target;
    const int maxHash; // maximum hash value, upper bound on number of states

    board(int _maxX,
          int _maxY,
          int _numColors,
          int _numBalls,
          vector<vector<bool>> _grid,
          vector<vector<vector<bool>>> _colorGrid,
          vector<vector<bool>> _initShade,
          vector<vector<int>> _button,
          pi _target);
    const vector<vector<int>> &getAdj() const { return adj; }
    const vector<int> &getDistance() const { return distance; }

    int hash(state s) const;
    state unhash(int h, bool checkValid) const;
    bool valid(state s) const;
    void initGraph();
    void analyzeGraph();
    bool checkWin(state s) const;
    static board randomBoard(int maxX, int maxY, int numColors, int numBalls);
    state randomState();
    int hint(int h) const;
    vector<int> solve(int hash) const;
};