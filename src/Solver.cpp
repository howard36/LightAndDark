#include <bits/stdc++.h>
#define pi pair<int, int>
#define ll long long
#define x first
#define y second
using namespace std;

#include "../include/Board.h"
#include "../include/State.h"

// int solutions = 0;

// void dfs(const state &s, vector<vector<int>> &adj, vector<bool> &visited, vector<vector<int>> &paths, vector<int> &path) {
//     if (s.getWin()) { // found a new solution
//         paths.push_back(path);
//         solutions++;
//         cout << "Found solution " << solutions << "\n";
//         return;
//     }
//     int h = s.getHash();
//     visited[h] = true;
//     for (int i = 0; i < s.b->numBalls * 4; i++) {
//         int next = adj[h][i];
//         if (!visited[next]) {
//             path.push_back(i);
//             dfs(s.b->unhash(next, true), adj, visited, paths, path);
//             path.pop_back();
//         }
//     }
//     visited[h] = false;
// }

// bool comp(vector<int> &a, vector<int> &b) {
//     return a.size() < b.size();
// }

// void solve(state s) {
//     cout << "In solve\n";
//     board b = *(s.b);
//     vector<vector<int>> adj = b.getGraph();
//     vector<bool> visited;
//     for (int i = 0; i < b.maxHash; i++) {
//         visited.push_back(false);
//     }
//     vector<vector<int>> paths;
//     vector<int> path;
//     dfs(s, adj, visited, paths, path);
//     if (paths.size() == 0) {
//         cout << "No solutions found\n";
//     }
//     else {
//         sort(paths.begin(), paths.end(), comp);
//         cout << "The shortest solution has " << paths[0].size() << " moves\n";
//         cout << paths.size() << " solutions found:\n";
//         for (int i = 0; i < paths.size(); i++) {
//             cout << i + 1 << ":";
//             for (int j : paths[i]) {
//                 cout << " " << j;
//             }
//             cout << "\n";
//         }
//     }
// }

int main() {
    board b = board::randomBoard(4, 4, 0, 2);
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
    s.solve();
    sampleState.solve();
}