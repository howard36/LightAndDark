#include "../include/Macros.h"

level::level() : b(board()), initState(0), difficulty(0) {
}

level::level(const state _s) : b(*(_s.b)), initState(_s.getHash()), difficulty(b.getDistance()[initState]) {
}

void level::play() const {
    int cur = initState;
    vi history;
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
                    printMove(hint);
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
                cur = initState;
                history.clear();
                history.push_back(cur);
                requestMove = false;
            }
            else if (ball >= 0 && ball < b.numBalls) {
                cin >> dir;
                if (dir >= 0 && dir < 4) {
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
                    printf("Invalid Move! dir = %d\n", dir);
                    requestMove = true;
                }
            }
            else { // invalid move
                printf("Invalid Move! ball = %d\n", ball);
                requestMove = true;
            }
        }
    }
    b.unhash(cur).print();
    printf("You Won!\n");
    printf("You took %d moves, the optimal solution is %d moves\n", (int)history.size() - 1, difficulty);
}

level level::hardLevel(int maxX, int maxY, int numColors, int numBalls, int minDifficulty) {
    int maxAttempts = 1000;
    for (int i = 0; i < maxAttempts; i++) {
        board b = board::randomBoard(maxX, maxY, numColors, numBalls);
        state s = b.unhash(b.getHardestState());
        level l = level(s);
        if (l.difficulty >= minDifficulty) {
            return l;
        }
    }
    printf("ERROR in hardLevel: could not generate a level with difficulty at least %d in %d attempts\n", minDifficulty, maxAttempts);
    return level();
}