#include "../include/Macros.h"

state::state() {
    b = nullptr;
    ballPos = vpi();
    colorFlip = vb();
    int hash = 0;
    bool win = false;
}

state::state(const board *_b, vpi _ballPos, vb _colorFlip, bool checkValid) { // uniform initialization works, but direct initialization doesn't??
    b = _b;
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

int state::occupied(pi pos) const {
    for (int i = 0; i < ballPos.size(); i++) {
        if (ballPos[i] == pos)
            return i;
    }
    return -1;
}

// dir = 0: down (positive x direction)
// dir = 1: left (positive y direction)
// dir = 2: up (negative x direction)
// dir = 3: right (negative y direction)
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
    else if (occupied(nextPos) != -1) {
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

int state::hint() const {
    return b->hint(hash);
}

vi state::solve() const {
    // printf("Solving the following state:\n");
    // print();
    return b->solve(hash);
}
