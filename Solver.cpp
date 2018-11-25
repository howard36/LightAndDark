#include <bits/stdc++.h>
#define pi pair<int, int>
#define x first
#define y second
using namespace std;

#include "Board.h"
#include "State.h"

void dfs(const state &s, vector<vector<int>> &adj, vector<bool> &visited, vector<vector<int>> &paths, vector<int> &path) {
    if (s.getWin()) { // found a new solution
        paths.push_back(path);
        return;
    }
    int h = s.getHash;
    visited[h] = true;
    for (int i = 0; i < s.b.numBalls * 4; i++) {
        int next = adj[h][i];
        if (!visited[next]) {
            path.push_back(i);
            dfs(s.b.unhash(next), adj, visited, paths, path);
            path.pop_back();
        }
    }
    visited[h] = false;
}

bool comp(vector<int> &a, vector<int> &b) {
    return a.size() < b.size();
}

void solve(state s) {
    board b = s.b;
    vector<vector<int>> adj = b.getGraph();
    vector<bool> visited;
    for (int i = 0; i < b.maxHash; i++) {
        visited.push_back(false);
    }
    vector<vector<int>> paths;
    vector<int> path;
    dfs(s, adj, visited, paths, path);
    if (paths.size() == 0) {
        cout << "No solutions found\n";
    }
    else {
        sort(paths.begin(), paths.end(), comp);
        cout << "The shortest solution has " << paths[0].size() << " moves\n";
        cout << paths.size() << " solutions found:\n";
        for (int i = 0; i<paths.size(); i++){
            cout << i+1 << ":";
            for (int j : paths[i]){
                cout << " " << j;
            }
            cout << "\n";
        }
    }
}

int main() {
    board b = board::randomBoard(5, 5, 2, 2);
    state s = b.randomState();
    solve(s);
}