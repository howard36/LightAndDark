#pragma once

#include <bits/stdc++.h>
using namespace std;

using vi = vector<int>;
using vvi = vector<vi>;

using vb = vector<bool>;
using vvb = vector<vb>;
using vvvb = vector<vvb>;

using pi = pair<int, int>;
using vpi = vector<pi>;
#define x first  // defines don't seem to work when a header includes Macros.h
#define y second // using and typedef work for headers, but not for x=first because first is not a data type

using ll = long long;
using vll = vector<ll>;
using vvll = vector<vll>;

using md = pair<pi, pi>; // move data, ball moves from first pi to second pi
using vmd = vector<md>;
using vvmd = vector<vmd>;

#include "../include/State.h"
#include "../include/Board.h"
#include "../include/Level.h"
#include "../include/Print.h"
