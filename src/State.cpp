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

pi next(pi pos, int dir) {
    int x = pos.x, y = pos.y;
    if (dir == 0)
        x++;
    else if (dir == 1)
        y++;
    else if (dir == 2)
        x--;
    else
        y--;
    return pi(x, y);
}

bool state::inGrid(pi p) const {
    return (p.x >= 0 && p.y >= 0 && p.x < b->maxX && p.y < b->maxY && b->grid[p.x][p.y]);
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

int state::isButton(pi pos) const {
    return b->button[pos.x][pos.y];
}

// dir = 0: down (positive x direction)
// dir = 1: left (positive y direction)
// dir = 2: up (negative x direction)
// dir = 3: right (negative y direction)
const pi state::rollBall(int ball, int dir) const {
    pi pos = ballPos[ball];
    if (pos == b->target)
        return pos;
    for (int i = 0; i < max(b->maxX, b->maxY); i++) { // check if upper bound is correct
        // check if ball stops after moving i squares in direction dir, or continues moving
        pi nextPos = next(pos, dir);
        if (nextPos == b->target)
            return nextPos;
        if (!inGrid(nextPos))
            return pos; // hits outer wall, cannot move
        if (getShade(pos) != getShade(nextPos))
            return pos; // next position is different shade
        if (occupied(nextPos) != -1)
            return pos; // next position contains another ball
        if (isButton(nextPos) != -1)
            return nextPos;
        // ball continues moving
        pos = nextPos;
    }
    printf("ERROR in state::move: ball did not stop after max(maxX, maxY) moves\n");
    return pi(-1, -1);
}

const pair<int, md> state::applyMove(int move) const {
    int ball = move / 4, dir = move % 4;
    pi newPos = rollBall(ball, dir);
    if (newPos == ballPos[ball]) {
        return make_pair(hash, md(ballPos[ball], newPos));
    }
    else {
        state s(*this);
        s.ballPos[ball] = newPos;
        int buttonColor = isButton(newPos);
        if (buttonColor != -1) {
            s.colorFlip[buttonColor] = !s.colorFlip[buttonColor];
        }
        return make_pair(s.b->hash(s), md(ballPos[ball], newPos));
    }
}

int state::hint() const {
    return b->hint(hash);
}

vi state::solve() const {
    // printf("Solving the following state:\n");
    // print();
    return b->solve(hash);
}
