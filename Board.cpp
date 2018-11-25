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