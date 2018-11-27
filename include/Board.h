#pragma once

#include "../include/Macros.h"

class board {
    vvi adj;
    vvmd moveData;
    vb validStates;
    vvll paths; // paths[i][j] = number of length i (not necessarily simple) paths from state j to winning state
    vi distance;
    int maxDist;
    int hardestState;

  public:
    const int maxX, maxY, numColors, numBalls;
    const vvb grid;       // maxX x maxY grid of bools representing which squares are part of the board
    const vvvb colorGrid; // colorGrid[c] is a 2d grid representing which squares have color c
    const vvb initShade;  // true = dark, false = light (initial values)
    const vvi button;     // button[x][y] = color of button at (x, y), or -1 if no button
    const pi target;
    const int maxHash; // maximum hash value, upper bound on number of states

    board();
    board(int _maxX,
          int _maxY,
          int _numColors,
          int _numBalls,
          vvb _grid,
          vvvb _colorGrid,
          vvb _initShade,
          vvi _button,
          pi _target);

    const vi &getDistance() const { return distance; }
    int hash(state s) const;
    state unhash(int h, bool checkValid = true) const;
    bool valid(state s) const;
    void initGraph();
    void analyzeGraph();
    bool checkWin(state s) const;
    bool checkWin(int h) const { return (distance[h] == 0); }
    static board randomBoard(int maxX, int maxY, int numColors, int numBalls);
    state randomState();
    int hint(int h) const;
    int move(int hash, int ball, int dir) const { return adj[hash][4 * ball + dir]; }
    int move(int hash, int move) const { return adj[hash][move]; }
    vi solve(int hash) const;
    void print() const;
    int getHardestState() const { return hardestState; }
};