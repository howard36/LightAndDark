#include <bits/stdc++.h>
#define pi pair<int, int>
#define ll long long
#define x first
#define y second
using namespace std;

#include "../include/Board.h"
#include "../include/State.h"

state::state(const board *_b, vector<pi> _ballPos, vector<bool> _colorFlip, bool checkValid) : b{_b} { // uniform initialization works, but direct initialization doesn't??
    ballPos = _ballPos;
    colorFlip = _colorFlip;
    if (checkValid && !b->valid(*this)) {
        cout << "ERROR in state contructor: state is invalid\n";
        return;
    }
    hash = b->hash(*this);
    win = b->checkWin(*this);
}

bool state::getShade(pi p) const {
    bool shade = b->initShade[p.x][p.y];
    for (int i = 0; i < b->numColors; i++) {
        if (b->colorGrid[i][p.x][p.y]) {
            shade ^= colorFlip[i];
        }
    }
    return shade;
}

bool state::occupied(pi p) const {
    for (pi pos : ballPos) {
        if (pos == p)
            return true;
    }
    return false;
}

// dir = 0: right (positive x direction)
// dir = 1: up (positive y direction)
// dir = 2: left (negative x direction)
// dir = 3: down (negative y direction)
// currently copies s on function call, fix this once a copy constructor is made
const state state::move(int ball, int dir) const {
    if (win)
        return *this;
    pi pos = ballPos[ball];
    if (pos == b->target)
        return *this;
    bool shade = getShade(pos);
    int x = pos.x, y = pos.y;
    if (dir == 0) {
        x++;
    }
    else if (dir == 1) {
        y++;
    }
    else if (dir == 2) {
        x--;
    }
    else {
        y--;
    }
    pi nextPos = pi(x, y);
    if (nextPos == b->target) {
        state s = state(*this);
        s.moveBall(ball, nextPos);
        return s;
    }
    else if (x < 0 || y < 0 || x >= b->maxX || y >= b->maxY || !b->grid[x][y]) {
        return *this; // next position is not in grid, cannot move
    }
    else if (shade != getShade(nextPos)) {
        return *this; // next position is different shade
    }
    else if (occupied(nextPos)) {
        return *this; // next position contains another ball
    }
    else if (b->button[x][y] != -1) {
        state s = state(*this);
        s.moveBall(ball, nextPos);
        s.flip(b->button[x][y]);
        return s;
    }
    else {
        state s = state(*this);
        s.moveBall(ball, nextPos);
        return s.move(ball, dir); // keep rolling in same direction
    }
}

const state state::move(int m) const {
    return move(m / 4, m % 4);
}

void state::flip(int color) {
    colorFlip[color] = !colorFlip[color];
    hash = b->hash(*this);
    win = b->checkWin(*this);
}

void state::moveBall(int ball, pi pos) {
    ballPos[ball] = pos;
    hash = b->hash(*this);
    win = b->checkWin(*this);
}

void state::printSquareRow(int x, int y, int r) const {
    // if (r == 1){
    //     if (b->colorGrid[0][x][y])
    // }

}

void state::print() const {
    printf("State:\n");
    for (int i = 0; i < ballPos.size(); i++) {
        printf("Ball %d is at (%d, %d)\n", i, ballPos[i].x, ballPos[i].y);
    }
    for (int i = 0; i < b->maxX; i++) {
        for (int j = 0; j < b->maxY; j++) {
            printf("%d ", (int)getShade(pi(i, j)));
        }
        printf("\n");
    }
}

int state::hint() const {
    return b->hint(hash);
}

vector<int> state::solve() const {
    printf("Solving the following state:\n");
    print();
    return b->solve(hash);
}
