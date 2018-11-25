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

void state::printSquareRow(int x, int y, int r) const {
    if (x < 0 || x >= b->maxX || y < 0 || y >= b->maxY){
        printf("ERROR in printSquareRow: invalid position\n");
        return;
    }
    if (r < 0 || r >= 4){
        printf("ERROR in printSquareRow: invalid row\n");
        return;
    }
    if (pi(x, y) == b->target) {
        if (r == 0){
            printf("        ");
        }
        else if (r == 1) {
            printf(" Target ");
        }
        else if (r == 2){
            if (b->numBalls > 0 && ballPos[0] == b->target){
                printf(" B0");
            }
            else {
                printf("   ");
            }
            printf("  ");
            if (b->numBalls > 1 && ballPos[1] == b->target){
                printf("B1 ");
            }
            else {
                printf("   ");
            }
        }
        else if (r == 3){
            if (b->numBalls > 2 && ballPos[2] == b->target){
                printf(" B2");
            }
            else {
                printf("   ");
            }
            printf("  ");
            if (b->numBalls > 3 && ballPos[3] == b->target){
                printf("B3 ");
            }
            else {
                printf("   ");
            }
        }
    }
    else if (!b->grid[x][y]) {
        printf("        ");
    }
    else {
        int shade = getShade(pi(x, y));
        char c = shade ? '/' : ' ';
        if (r == 0) {
            if (b->numColors > 0 && b->colorGrid[0][x][y]) {
                printf("0 ");
            }
            else {
                printf("%c%c", c, c);
            }
            printf("%c%c%c%c", c, c, c, c);
            if (b->numColors > 1 && b->colorGrid[1][x][y]) {
                printf(" 1");
            }
            else {
                printf("%c%c", c, c);
            }
        }
        else if (r == 1) {
            if (b->button[x][y] == -1) {
                printf("%c%c%c%c%c%c%c%c", c, c, c, c, c, c, c, c);
            }
            else {
                printf("%c%c %d %c%c%c", c, c, b->button[x][y], c, c, c);
            }
        }
        else if (r == 2) {
            if (occupied(pi(x, y)) == -1) {
                printf("%c%c%c%c%c%c%c%c", c, c, c, c, c, c, c, c);
            }
            else {
                printf("%c%c B%d %c%c", c, c, occupied(pi(x, y)), c, c);
            }
        }
        else if (r == 3) {
            if (b->numColors > 2 && b->colorGrid[2][x][y]) {
                printf("2 ");
            }
            else {
                printf("%c%c", c, c);
            }
            printf("%c%c%c%c", c, c, c, c);
            if (b->numColors > 3 && b->colorGrid[3][x][y]) {
                printf(" 3");
            }
            else {
                printf("%c%c", c, c);
            }
        }
    }
}

// x in [0, maxX], y in [0, maxY]
void state::printCorner(int x, int y) const {
    bool upleft = b->grid[max(0, x - 1)][max(0, y - 1)];
    bool upright = b->grid[max(0, x - 1)][min(b->maxY - 1, y)];
    bool downleft = b->grid[min(b->maxX - 1, x)][max(0, y - 1)];
    bool downright = b->grid[min(b->maxX - 1, x)][min(b->maxY - 1, y)];
    if (upleft || upright || downleft || downright) {
        printf("-");
    }
    else {
        printf(" ");
    }
}

// x in [0, maxX], y in [0, maxY)
void state::printHorizontalEdge(int x, int y) const {
    bool up = b->grid[max(0, x - 1)][y];
    bool down = b->grid[min(b->maxX - 1, x)][y];
    if (up || down) {
        printf("--------");
    }
    else {
        printf("        ");
    }
}

// x in [0, maxX), y in [0, maxY]
void state::printVerticalEdge(int x, int y) const {
    bool left = b->grid[x][max(0, y - 1)];
    bool right = b->grid[x][min(b->maxY - 1, y)];
    if (left || right) {
        printf("|");
    }
    else {
        printf(" ");
    }
}

void state::print() const {
    printf("\n");
    for (int x = 0; x < b->maxX; x++) {
        for (int y = 0; y < b->maxY; y++) {
            printCorner(x, y);
            printHorizontalEdge(x, y);
        }
        printCorner(x, b->maxY);
        printf("\n");
        for (int r = 0; r < 4; r++) {
            for (int y = 0; y < b->maxY; y++) {
                printVerticalEdge(x, y);
                printSquareRow(x, y, r);
            }
            printVerticalEdge(x, b->maxY);
            printf("\n");
        }
    }
    for (int y = 0; y < b->maxY; y++) {
        printCorner(b->maxX, y);
        printHorizontalEdge(b->maxX, y);
    }
    printCorner(b->maxX, b->maxY);
    printf("\n\n");
}

int state::hint() const {
    return b->hint(hash);
}

vector<int> state::solve() const {
    printf("Solving the following state:\n");
    print();
    return b->solve(hash);
}
