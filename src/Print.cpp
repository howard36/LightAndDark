#include "../include/Macros.h"

// General Print

void printVector(vi v) {
    for (int i : v) {
        printf("%d ", i);
    }
    printf("\n");
}

void printVector(vb v) {
    for (bool b : v) {
        printf("%d ", (int)b);
    }
    printf("\n");
}

void print2dVector(vvi v) {
    for (int i = 0; i < v.size(); i++) {
        printVector(v[i]);
    }
}

void print2dVector(vvb v) {
    for (int i = 0; i < v.size(); i++) {
        printVector(v[i]);
    }
}

// Moves and Solutions

void printMove(int move) {
    if (move < 0) {
        printf("ERROR in printMove: move is invalid\n");
    }
    int ball = move / 4, dir = move % 4;
    printf("Move ball %d ", ball);
    if (dir == 0) {
        printf("down (0)\n");
    }
    else if (dir == 1) {
        printf("right (1)\n");
    }
    else if (dir == 2) {
        printf("up (2)\n");
    }
    else {
        printf("left (3)\n");
    }
}

void printSolution(vi solution) {
    for (int move : solution) {
        printMove(move);
    }
}

// States

void state::printSquareRow(int x, int y, int r) const {
    if (x < 0 || x >= b->maxX || y < 0 || y >= b->maxY) {
        printf("ERROR in printSquareRow: invalid position\n");
        return;
    }
    if (r < 0 || r >= 4) {
        printf("ERROR in printSquareRow: invalid row\n");
        return;
    }
    if (pi(x, y) == b->target) {
        if (r == 0) {
            printf("        ");
        }
        else if (r == 1) {
            printf(" Target ");
        }
        else if (r == 2) {
            if (b->numBalls > 0 && ballPos[0] == b->target) {
                printf(" B0");
            }
            else {
                printf("   ");
            }
            printf("  ");
            if (b->numBalls > 1 && ballPos[1] == b->target) {
                printf("B1 ");
            }
            else {
                printf("   ");
            }
        }
        else if (r == 3) {
            if (b->numBalls > 2 && ballPos[2] == b->target) {
                printf(" B2");
            }
            else {
                printf("   ");
            }
            printf("  ");
            if (b->numBalls > 3 && ballPos[3] == b->target) {
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

// Print Boards (not used anymore)

void board::print() const {
    printf("Target: (%d, %d)\n", target.x, target.y);
    printf("Grid:\n");
    print2dVector(grid);
    for (int i = 0; i < numColors; i++) {
        printf("ColorGrid[%d]:\n", i);
        print2dVector(colorGrid[i]);
    }
    // printf("InitShade:\n");
    // print2dVector(initShade);
    printf("Button:\n");
    print2dVector(button);
}