#include <bits/stdc++.h>
#define pi pair<int, int>
#define x first
#define y second
using namespace std;

#include <Board.h>
#include <State.h>

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

state board::unhash(int h) const {
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
    return state(*this, ballPos, colorFlip);
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
            if (s.getBallPos()[i] == s.getBallPos()[j])
                return false;
        }
    }
}

vector<vector<int>> board::getGraph() const {
    vector<vector<int>> adj(maxHash, vector<int>(numBalls * 4));
    for (int i = 0; i < maxHash; i++) {
        state s = unhash(i);
        if (valid(s)) {
            for (int j = 0; j < numBalls; j++) {
                for (int k = 0; k < 4; k++) {
                    adj[i][4 * j + k] = hash(s.move(j, k));
                }
            }
        }
    }
    return adj;
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
            if (button[i][j] || target == pi(i, j))
                continue;
            squares.push_back(pi(i, j));
        }
    }
    random_shuffle(squares.begin(), squares.end());
    if (squares.size() < numBalls){
        cout << "ERROR in random state generator: not enough space to place balls\n";
    }
    vector<pi> ballPos;
    for (int i = 0; i<numBalls; i++){
        ballPos.push_back(squares[i]);
    }
    vector<bool> colorFlip(numColors, false);
    return state(*this, ballPos, colorFlip);
}