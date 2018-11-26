#include "../include/Macros.h"
#include "../include/Level.h"

int main() {
    level l = level::hardLevel(5, 5, 3, 2, 30);
    printf("Playing Hard Level:\n");
    l.play();

    // vvb grid{
    //     {0, 0, 1, 1, 1},
    //     {0, 1, 1, 1, 1},
    //     {1, 1, 1, 1, 1},
    //     {1, 1, 1, 1, 0},
    //     {1, 1, 1, 0, 0}};
    // vvvb colorGrid{
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
    // vvb initShade{
    //     {0, 0, 0, 1, 0},
    //     {0, 1, 0, 1, 0},
    //     {0, 1, 0, 1, 0},
    //     {0, 0, 1, 0, 0},
    //     {0, 1, 1, 0, 0}};
    // vvi button{
    //     {-1, -1, 0, -1, -1},
    //     {-1, -1, -1, -1, -1},
    //     {1, -1, -1, -1, 1},
    //     {-1, -1, -1, -1, -1},
    //     {-1, -1, 0, -1, -1}};
    // board sampleBoard = board(5, 5, 2, 2, grid, colorGrid, initShade, button, pi(2, 2));
    // vpi ballPos{pi(4, 0), pi(0, 4)};
    // vb colorFlip{0, 0};
    // state sampleState = state(&sampleBoard, ballPos, colorFlip, true);
    // level sampleLevel = level(sampleState);

    // printf("Playing Sample Level:\n");
    // sampleLevel.play();
}