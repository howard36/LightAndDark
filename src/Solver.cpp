#include "../include/Macros.h"

int main() {
    board b = board::randomBoard(5, 5, 2, 2);
    state s = b.randomState();

    vvb grid{
        {0, 0, 1, 1, 1},
        {0, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 0},
        {1, 1, 1, 0, 0}};
    vvvb colorGrid{
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
    vvb initShade{
        {0, 0, 0, 1, 0},
        {0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0}};
    vvi button{
        {-1, -1, 0, -1, -1},
        {-1, -1, -1, -1, -1},
        {1, -1, -1, -1, 1},
        {-1, -1, -1, -1, -1},
        {-1, -1, 0, -1, -1}};
    board sampleBoard = board(5, 5, 2, 2, grid, colorGrid, initShade, button, pi(2, 2));
    vpi ballPos{pi(4, 0), pi(0, 4)};
    vb colorFlip{0, 0};
    state sampleState = state(&sampleBoard, ballPos, colorFlip, true);
    level sampleLevel = level(sampleState);

    sampleLevel.play();
}