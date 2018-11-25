#include <bits/stdc++.h>
#define pi pair<int, int>
#define ll long long
#define x first
#define y second
using namespace std;

#include "../include/Board.h"
#include "../include/State.h"

int main() {
    board b = board::randomBoard(5, 5, 2, 2);
    b.print();
    state s = b.randomState();
    // vector<vector<bool>> grid{
    //     {0, 0, 1, 1, 1},
    //     {0, 1, 1, 1, 1},
    //     {1, 1, 1, 1, 1},
    //     {1, 1, 1, 1, 0},
    //     {1, 1, 1, 0, 0}};
    // vector<vector<vector<bool>>> colorGrid{
    //     {{0, 0, 0, 1, 0},
    //      {0, 0, 0, 0, 1},
    //      {0, 1, 0, 0, 1},
    //      {0, 0, 0, 0, 0},
    //      {0, 0, 0, 0, 0}},
    //     {{0, 0, 0, 1, 0},
    //      {0, 0, 1, 0, 0},
    //      {0, 0, 0, 1, 0},
    //      {1, 0, 0, 0, 0},
    //      {0, 1, 1, 0, 0}}};
    // vector<vector<bool>> initShade{
    //     {0, 0, 1, 0, 1},
    //     {0, 0, 1, 0, 1},
    //     {1, 0, 1, 0, 1},
    //     {1, 1, 0, 1, 0},
    //     {1, 0, 0, 0, 0}};
    // vector<vector<int>> button{
    //     {-1, -1, 0, -1, -1},
    //     {-1, -1, -1, -1, -1},
    //     {1, -1, -1, -1, 1},
    //     {-1, -1, -1, -1, -1},
    //     {-1, -1, 0, -1, -1}};
    // board sampleBoard = board(5, 5, 2, 2, grid, colorGrid, initShade, button, pi(2, 2));
    // vector<pi> ballPos{pi(4, 0), pi(0, 4)};
    // vector<bool> colorFlip{0, 0};
    // state sampleState = state(&sampleBoard, ballPos, colorFlip, true);
    s.solve();
    // printf("----------------------------------------------------------------------------\n");
    // printf("Sample State:\n");
    // sampleState.solve();
}