#pragma once

#include "../include/Macros.h"

class board;

class state {
    vpi ballPos;  // position of each ball
    vb colorFlip; // whether each color has been flipped from initial shade or not
    int hash;
    bool win; // whether the state is a winning state (all balls in target)
  public:
    const board *b; // constant pointer does not ensure board is constant

    state();
    state(const board *_b, vpi _ballPos, vb _colorFlip, bool checkValid);
    vpi getBallPos() const { return ballPos; }
    vb getColorFlip() const { return colorFlip; }
    int getHash() const { return hash; }
    bool getWin() const { return win; }

    bool getShade(pi p) const;
    int occupied(pi pos) const;
    const state move(int m) const;
    const state move(int ball, int dir) const;
    void flip(int color);
    void moveBall(int ball, pi pos);
    void printSquareRow(int x, int y, int r) const;
    void printCorner(int x, int y) const;
    void printHorizontalEdge(int x, int y) const;
    void printVerticalEdge(int x, int y) const;
    void print() const;
    int hint() const;
    vi solve() const;
};