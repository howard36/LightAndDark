#pragma once

#include <bits/stdc++.h>
#define pi pair<int, int>
#define ll long long
#define x first
#define y second
using namespace std;

#include "../include/Board.h"
#include "../include/State.h"

class level {
    board b;
    state initState;

  public:
    level(state _s);
    void play() const;
};