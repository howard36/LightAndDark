#include "../include/Macros.h"

board::board() : maxX(0),
                 maxY(0),
                 numColors(0),
                 numBalls(0),
                 grid(vvb()),
                 colorGrid(vvvb()),
                 initShade(vvb()),
                 button(vvi()),
                 target(pi()),
                 maxHash(0) {
}

board::board(int _maxX,
             int _maxY,
             int _numColors,
             int _numBalls,
             vvb _grid,
             vvvb _colorGrid,
             vvb _initShade,
             vvi _button,
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
    vpi ballPos(numBalls);
    vb colorFlip(numColors);
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
    adj = vvi(maxHash, vi(4 * numBalls));
    moveData = vvmd(maxHash, vmd(4 * numBalls));
    validStates = vb(maxHash, false);
    for (int i = 0; i < maxHash; i++) {
        state s = unhash(i, false);
        if (valid(s)) {
            validStates[i] = true;
            for (int j = 0; j < 4 * numBalls; j++) {
                auto data = s.applyMove(j);
                adj[i][j] = data.first;
                moveData[i][j] = data.second;
            }
        }
    }
}

void board::analyzeGraph() {
    paths.push_back(vll(maxHash, 0));
    distance = vi(maxHash, -1);
    for (int i = 0; i < maxHash; i++) {
        if (unhash(i, false).getWin()) {
            paths[0][i] = 1;
            distance[i] = 0;
        }
    }
    for (int i = 1; i < maxHash; i++) {
        paths.push_back(vll(maxHash, 0));
        bool addNew = false;
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
                addNew = true;
                maxDist = i;
                hardestState = j;
            }
        }
        if (!addNew) {
            break;
        }
    }
    // cout << "The hardest state has a solution of length " << maxDist << "\n";
    // cout << "This is the hardest state:\n";
    // unhash(hardestState).print();
}

bool board::checkWin(state s) const {
    for (pi pos : s.getBallPos()) {
        if (pos != target)
            return false;
    }
    return true;
}

board board::randomBoard(int maxX, int maxY, int numColors, int numBalls) {
    vvb grid(maxX, vb(maxY, true));
    vvvb colorGrid(numColors, vvb(maxX, vb(maxY, false)));
    for (int i = 0; i < numColors; i++) {
        for (int j = 0; j < maxX; j++) {
            for (int k = 0; k < maxY; k++) {
                colorGrid[i][j][k] = (bool)(rand() % 3 == 0); // each color covers around 1/3 of the board
            }
        }
    }
    vvb initShade(maxX, vb(maxY, false));
    for (int i = 0; i < maxX; i++) {
        for (int j = 0; j < maxY; j++) {
            initShade[i][j] = (bool)(rand() % 2); // equal amounts of light and dark
        }
    }
    vvi button(maxX, vi(maxY, -1));
    vpi squares;
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
    int inGrid = 0;
    for (int i = 0; i < maxX; i++) {
        for (int j = 0; j < maxY; j++) {
            inGrid += (int)grid[i][j];
        }
    }
    int extra = rand() % ((inGrid - numColors - numBalls - 1) / 5);
    for (int i = 0; i < extra; i++) {
        button[squares[i + numColors + 1].x][squares[i + numColors + 1].y] = rand() % numColors;
    }
    return board(maxX, maxY, numColors, numBalls, grid, colorGrid, initShade, button, target);
}

state board::randomState() {
    vpi squares;
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
    vpi ballPos;
    for (int i = 0; i < numBalls; i++) {
        ballPos.push_back(squares[i]);
    }
    vb colorFlip(numColors, false);
    return state(this, ballPos, colorFlip, true);
}

int board::hint(int hash) const {
    int d = distance[hash];
    if (d == -1) {
        return -1;
    }
    else if (d == 0) {
        return -2;
    }
    else {
        vi bestMoves;
        for (int i = 0; i < 4 * numBalls; i++) {
            if (distance[adj[hash][i]] == d - 1) {
                bestMoves.push_back(i);
            }
        }
        return bestMoves[rand() % bestMoves.size()];
    }
}

vi board::solve(int hash) const {
    vi moves;
    if (distance[hash] == -1) {
        cout << "Impossible to solve from this state\n";
        return moves;
    }
    while (distance[hash] > 0) {
        int bestMove = hint(hash);
        moves.push_back(bestMove);
        hash = adj[hash][bestMove];
    }
    cout << "Solution size is " << moves.size() << "\n";
    printSolution(moves);
    return moves;
}