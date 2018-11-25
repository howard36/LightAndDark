#include <bits/stdc++.h>
#define pi pair<int, int>
#define ll long long
#define x first
#define y second
using namespace std;

#include "../include/Board.h"
#include "../include/State.h"

board::board(int _maxX,
             int _maxY,
             int _numColors,
             int _numBalls,
             vector<vector<bool>> _grid,
             vector<vector<vector<bool>>> _colorGrid,
             vector<vector<bool>> _initShade,
             vector<vector<int>> _button,
             pi _target) : maxX(_maxX),
                           maxY(_maxY),
                           numColors(_numColors),
                           numBalls(_numBalls),
                           grid(_grid),
                           colorGrid(_colorGrid),
                           initShade(_initShade),
                           button(_button),
                           target(_target),
                           maxHash((int)pow(maxX * maxY, numBalls) << numColors) {
    initGraph();
    analyzeGraph();
}

int board::hash(state s) const {
    int h = 0;
    for (int i = 0; i < numBalls; i++) {
        h = maxX * h + s.getBallPos()[i].x;
        h = maxY * h + s.getBallPos()[i].y;
    }
    for (int i = 0; i < numColors; i++) {
        h = 2 * h + (int)s.getColorFlip()[i];
    }
    return h;
}

state board::unhash(int h, bool checkValid) const {
    vector<pi> ballPos(numBalls);
    vector<bool> colorFlip(numColors);
    for (int i = numColors - 1; i >= 0; i--) {
        colorFlip[i] = (bool)(h % 2);
        h /= 2;
    }
    for (int i = numBalls - 1; i >= 0; i--) {
        int y = h % maxY;
        h /= maxY;
        int x = h % maxX;
        h /= maxX;
        ballPos[i] = pi(x, y);
    }
    state s = state(this, ballPos, colorFlip, checkValid);
    return s;
}

bool board::valid(state s) const {
    if (s.getBallPos().size() != numBalls)
        return false;
    if (s.getColorFlip().size() != numColors)
        return false;
    for (int i = 0; i < numBalls; i++) {
        pi p = s.getBallPos()[i];
        if (p.x < 0 || p.x >= maxX || p.y < 0 || p.y >= maxY)
            return false;
        if (grid[p.x][p.y] == 0)
            return false;
    }
    for (int i = 0; i < numBalls; i++) {
        for (int j = i + 1; j < numBalls; j++) {
            if (s.getBallPos()[i] == s.getBallPos()[j] && s.getBallPos()[i] != target)
                return false;
        }
    }
    return true;
}

void board::initGraph() {
    adj = vector<vector<int>>(maxHash, vector<int>(numBalls * 4));
    validStates = vector<bool>(maxHash, false);
    for (int i = 0; i < maxHash; i++) {
        state s = unhash(i, false);
        if (valid(s)) {
            validStates[i] = true;
            for (int j = 0; j < 4 * numBalls; j++) {
                adj[i][j] = hash(s.move(j));
            }
        }
    }
}

void board::analyzeGraph() {
    paths = vector<vector<ll>>(maxHash, vector<ll>(maxHash, 0));
    distance = vector<int>(maxHash, -1);
    for (int i = 0; i < maxHash; i++) {
        if (checkWin(unhash(i, false))) {
            paths[0][i] = 1;
            distance[i] = 0;
        }
    }
    for (int i = 1; i < maxHash; i++) {
        for (int j = 0; j < maxHash; j++) {
            if (!validStates[j])
                continue;
            for (int k = 0; k < 4 * numBalls; k++) {
                if (adj[j][k] != j) {
                    paths[i][j] += paths[i - 1][adj[j][k]];
                }
            }
            if (distance[j] == -1 && paths[i][j] > 0) {
                distance[j] = i;
            }
        }
    }
    maxDist = -1;
    for (int i = 0; i < maxHash; i++) {
        if (distance[i] > maxDist) {
            maxDist = distance[i];
            hardestState = i;
        }
    }
    cout << "The hardest state has a solution of length " << maxDist << "\n";
    cout << "This is the hardest state:\n";
    unhash(hardestState).solve();
}

bool board::checkWin(state s) const {
    for (pi pos : s.getBallPos()) {
        if (pos != target)
            return false;
    }
    return true;
}

board board::randomBoard(int maxX, int maxY, int numColors, int numBalls) {
    srand(time(NULL));
    vector<vector<bool>> grid(maxX, vector<bool>(maxY, true));
    vector<vector<vector<bool>>> colorGrid(numColors, vector<vector<bool>>(maxX, vector<bool>(maxY, false)));
    for (int i = 0; i < numColors; i++) {
        for (int j = 0; j < maxX; j++) {
            for (int k = 0; k < maxY; k++) {
                colorGrid[i][j][k] = (bool)(rand() % 3 == 0); // each color covers around 1/3 of the board
            }
        }
    }
    vector<vector<bool>> initShade(maxX, vector<bool>(maxY, false));
    for (int i = 0; i < maxX; i++) {
        for (int j = 0; j < maxY; j++) {
            initShade[i][j] = (bool)(rand() % 2); // equal amounts of light and dark
        }
    }
    vector<vector<int>> button(maxX, vector<int>(maxY, -1));
    vector<pi> squares;
    for (int i = 0; i < maxX; i++) {
        for (int j = 0; j < maxY; j++) {
            squares.push_back(pi(i, j));
        }
    }
    random_shuffle(squares.begin(), squares.end());
    pi target = squares[0];
    for (int i = 0; i < numColors; i++) {
        button[squares[i + 1].x][squares[i + 1].y] = i;
    }
    int extra = rand() % ((maxX * maxY - numColors - numBalls - 1) / 10);
    for (int i = 0; i < extra; i++) {
        button[squares[i + numColors + 1].x][squares[i + numColors + 1].y] = rand() % numColors;
    }
    return board(maxX, maxY, numColors, numBalls, grid, colorGrid, initShade, button, target);
}

state board::randomState() {
    srand(time(NULL));
    vector<pi> squares;
    for (int i = 0; i < maxX; i++) {
        for (int j = 0; j < maxY; j++) {
            if (button[i][j] != -1 || target == pi(i, j))
                continue;
            squares.push_back(pi(i, j));
        }
    }
    random_shuffle(squares.begin(), squares.end());
    if (squares.size() < numBalls) {
        cout << "ERROR in random state generator: not enough space to place balls\n";
    }
    vector<pi> ballPos;
    for (int i = 0; i < numBalls; i++) {
        ballPos.push_back(squares[i]);
    }
    vector<bool> colorFlip(numColors, false);
    return state(this, ballPos, colorFlip, true);
}

int board::hint(int hash) const {
    srand(time(NULL));
    int d = distance[hash];
    if (d == -1) {
        return -1;
    }
    else if (d == 0) {
        return -2;
    }
    else {
        vector<int> bestMoves;
        for (int i = 0; i < 4 * numBalls; i++) {
            if (distance[adj[hash][i]] == d - 1) {
                bestMoves.push_back(i);
            }
        }
        return bestMoves[rand() % bestMoves.size()];
    }
}

void printMove(int move) {
    int ball = move / 4, dir = move % 4;
    cout << "Move ball " << ball << " ";
    if (dir == 0) {
        cout << "down\n";
    }
    else if (dir == 1) {
        cout << "right\n";
    }
    else if (dir == 2) {
        cout << "up\n";
    }
    else if (dir == 3) {
        cout << "left\n";
    }
    else {
        cout << "ERROR when printing move: invalid direction\n";
    }
}

void printSolution(vector<int> solution) {
    for (int move : solution) {
        printMove(move);
    }
}

vector<int> board::solve(int hash) const {
    vector<int> moves;
    if (distance[hash] == -1) {
        cout << "Impossible to solve from this state\n";
        return moves;
    }
    while (distance[hash] > 0) {
        int bestMove = hint(hash);
        // printMove(bestMove);
        moves.push_back(bestMove);
        hash = adj[hash][bestMove];
        // unhash(hash).print();
    }
    cout << "Solution size is " << moves.size() << "\n";
    printSolution(moves);
    return moves;
}

void print2dVector(vector<vector<int>> v){
    for (int i = 0; i<v.size(); i++){
        for (int j = 0; j<v[0].size(); j++){
            printf("%d ", v[i][j]);
        }
        printf("\n");
    }
}

void print2dVector(vector<vector<bool>> v){
    for (int i = 0; i<v.size(); i++){
        for (int j = 0; j<v[0].size(); j++){
            printf("%d ", (int)v[i][j]);
        }
        printf("\n");
    }
}

void board::print() const {
    printf("Target: (%d, %d)\n", target.x, target.y);
    printf("Grid:\n");
    print2dVector(grid);
    for (int i = 0; i<numColors; i++){
        printf("ColorGrid[%d]:\n", i);
        print2dVector(colorGrid[i]);
    }
    // printf("InitShade:\n");
    // print2dVector(initShade);
    printf("Button:\n");
    print2dVector(button);
}