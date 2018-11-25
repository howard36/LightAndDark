#include <bits/stdc++.h>
#define pi pair<int, int>
#define ll long long
#define x first
#define y second
using namespace std;

#include "../include/Board.h"
#include "../include/State.h"

void printMove2(int move) {
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

void play(state initState) {
    board b = *(initState.b);
    int cur = initState.getHash();
    vector<int> history;
    history.push_back(cur);
    int ball, dir;
    while (!b.checkWin(cur)) {
        printf("-------------------------------------------\n");
        printf("Current State:\n");
        b.unhash(cur).print();
        bool requestMove = true;
        while (requestMove) {
            printf("Enter your move: ");
            cin >> ball;
            if (ball == -1) {
                int hint = b.hint(cur);
                printf("Hint: ");
                if (hint == -1) {
                    printf("The current state is impossible to solve, you should restart\n");
                }
                else if (hint == -2) {
                    printf("ERROR in play: hint = -2, but current position is not winning\n");
                }
                else {
                    printMove2(hint);
                    requestMove = true;
                }
            }
            else if (ball == -2) { // undo move
                if (history.size() == 1) {
                    printf("ERROR in play: no moves to undo\n");
                    requestMove = true;
                }
                else {
                    printf("You undid your last move\n");
                    history.pop_back();
                    cur = history[history.size() - 1];
                    requestMove = false;
                }
            }
            else if (ball == -3) { // reset to initial state
                cur = initState.getHash();
                history.clear();
                history.push_back(cur);
                requestMove = false;
            }
            else if (ball >= 0 && ball < b.numBalls && dir >= 0 && dir < 4) {
                cin >> dir;
                cur = b.move(cur, ball, dir);
                if (cur == history[history.size() - 1]) { // move had no effect
                    printf("Invalid Move! (move had no effect)\n");
                    requestMove = true;
                }
                else {
                    history.push_back(cur);
                    requestMove = false;
                }
            }
            else { // invalid move
                printf("Invalid Move!\n");
                requestMove = true;
            }
        }
    }
    b.unhash(cur).print();
    printf("You Won!\n");
}

int main() {
    board b = board::randomBoard(5, 5, 2, 2);
    // b.print();
    state s = b.randomState();
    vector<vector<bool>> grid{
        {0, 0, 1, 1, 1},
        {0, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 0},
        {1, 1, 1, 0, 0}};
    vector<vector<vector<bool>>> colorGrid{
        {{0, 0, 0, 1, 0},
         {0, 0, 0, 0, 1},
         {0, 1, 0, 0, 1},
         {0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0}},
        {{0, 0, 0, 1, 0},
         {0, 0, 1, 0, 0},
         {0, 0, 0, 1, 0},
         {1, 0, 0, 0, 0},
         {0, 1, 1, 0, 0}}};
    vector<vector<bool>> initShade{
        {0, 0, 1, 0, 1},
        {0, 0, 1, 0, 1},
        {1, 0, 1, 0, 1},
        {1, 1, 0, 1, 0},
        {1, 0, 0, 0, 0}};
    vector<vector<int>> button{
        {-1, -1, 0, -1, -1},
        {-1, -1, -1, -1, -1},
        {1, -1, -1, -1, 1},
        {-1, -1, -1, -1, -1},
        {-1, -1, 0, -1, -1}};
    board sampleBoard = board(5, 5, 2, 2, grid, colorGrid, initShade, button, pi(2, 2));
    vector<pi> ballPos{pi(4, 0), pi(0, 4)};
    vector<bool> colorFlip{0, 0};
    state sampleState = state(&sampleBoard, ballPos, colorFlip, true);
    // s.solve();
    // printf("----------------------------------------------------------------------------\n");
    // printf("Sample State:\n");
    // sampleState.solve();
    play(sampleState);
}