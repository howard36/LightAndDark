function pairEqual(a, b) {
    return a[0] == b[0] && a[1] == b[1];
}

// random integer from 0 to max-1 inclusive
function random(max) {
    return Math.floor(Math.random() * max)
}

function randomShuffle(arr) {
    for (var i = 0; i < arr.length - 1; i++) {
        var r = random(arr.length - i);
        var temp = arr[i];
        arr[i] = arr[i + r];
        arr[i + r] = temp;
    }
}


class Board {
    constructor(maxX, maxY, numColors, numBalls, grid, colorGrid, initShade, button, target) {
        this.maxX = maxX;
        this.maxY = maxY;
        this.numColors = numColors;
        this.numBalls = numBalls;
        this.grid = grid;
        this.colorGrid = colorGrid;
        this.initShade = initShade;
        this.button = button;
        this.target = target;
        this.maxHash = Math.pow(maxX * maxY, numBalls) * Math.pow(2, numColors);
        this.initGraph();
        this.analyzeGraph();
    }

    getDistance() { return this.distance; }
    move(hash, ball, dir) { return this.adj[hash][4 * ball + dir]; }

    static copy(board) {
        console.log("Copying Board");
        var maxX = board.maxX, maxY = board.maxY, numColors = board.numColors, numBalls = board.numBalls;
        var grid = JSON.parse(JSON.stringify(board.grid));
        var colorGrid = JSON.parse(JSON.stringify(board.colorGrid));
        var initShade = JSON.parse(JSON.stringify(board.initShade));
        var button = JSON.parse(JSON.stringify(board.button));
        var target = JSON.parse(JSON.stringify(board.target));
        return new Board(maxX, maxY, numColors, numBalls, grid, colorGrid, initShade, button, target);
    }

    hash(s) {
        var h = 0;
        for (var i = 0; i < this.numBalls; i++) {
            h = this.maxX * h + s.getBallPos()[i][0];
            h = this.maxY * h + s.getBallPos()[i][1];
        }
        for (var i = 0; i < this.numColors; i++) {
            h = 2 * h + Number(s.getColorFlip()[i]);
        }
        return h;
    }

    unhash(h) {
        var colorFlip = new Array(this.numColors);
        for (var i = this.numColors - 1; i >= 0; i--) {
            colorFlip[i] = Boolean(h % 2);
            h = Math.floor(h / 2);
        }
        var ballPos = new Array(this.numBalls);
        for (var i = this.numBalls - 1; i >= 0; i--) {
            var y = h % this.maxY;
            h = Math.floor(h / this.maxY);
            var x = h % this.maxX;
            h = Math.floor(h / this.maxX);
            ballPos[i] = [x, y];
        }
        return new State(this, ballPos, colorFlip);
    }

    valid(s) {
        if (s.getBallPos().length != this.numBalls)
            return false;
        if (s.getColorFlip().length != this.numColors)
            return false;
        for (var i = 0; i < this.numBalls; i++) {
            var p = s.getBallPos()[i];
            if (p[0] < 0 || p[0] >= this.maxX || p[1] < 0 || p[1] >= this.maxY)
                return false;
            // console.log(`(${p[0]}, ${p[1]})`);
            // if (!Number.isInteger(p[0])){
            //     console.log(s.getBallPos());
            //     console.log(s);
            // }
            if (this.grid[p[0]][p[1]] == 0)
                return false;
        }
        for (var i = 0; i < this.numBalls; i++) {
            for (var j = i + 1; j < this.numBalls; j++) {
                if (pairEqual(s.getBallPos()[i], s.getBallPos()[j]) && !pairEqual(s.getBallPos()[i], target))
                    return false;
            }
        }
        return true;
    }

    initGraph() {
        this.adj = [];
        this.moveData = [];
        this.validStates = new Array(this.maxHash).fill(false);
        for (var i = 0; i < this.maxHash; i++) {
            this.adj.push(new Array(4 * this.numBalls));
            this.moveData.push(new Array(4 * this.numBalls));
            var s = this.unhash(i);
            if (this.valid(s)) {
                this.validStates[i] = true;
                for (var j = 0; j < 4 * this.numBalls; j++) {
                    var data = s.applyMove(j);
                    this.adj[i][j] = data[0];
                    this.moveData[i][j] = data[1];
                }
            }
        }
    }

    analyzeGraph() {
        this.paths = [];
        this.paths.push(new Array(this.maxHash).fill(0));
        this.distance = new Array(this.maxHash).fill(-1);
        for (var i = 0; i < this.maxHash; i++) {
            if (this.unhash(i).getWin()) {
                this.paths[0][i] = 1;
                this.distance[i] = 0;
            }
        }
        for (var i = 1; i < this.maxHash; i++) {
            this.paths.push(new Array(this.maxHash).fill(0));
            var addNew = false;
            for (var j = 0; j < this.maxHash; j++) {
                if (!this.validStates[j])
                    continue;
                for (var k = 0; k < 4 * this.numBalls; k++) {
                    if (this.adj[j][k] != j) {
                        this.paths[i][j] += this.paths[i - 1][this.adj[j][k]];
                    }
                }
                if (this.distance[j] == -1 && this.paths[i][j] > 0) {
                    this.distance[j] = i;
                    addNew = true;
                    this.maxDist = i;
                    this.hardestState = j;
                }
            }
            if (!addNew) {
                break;
            }
        }
    }

    checkWin(s) {
        for (var i = 0; i < this.numBalls; i++) {
            if (!pairEqual(s.getBallPos()[i], target))
                return false;
        }
        return true;
    }

    static randomBoard(maxX, maxY, numColors, numBalls) {
        var grid = new Array(maxX);
        for (var i = 0; i < maxX; i++) {
            grid[i] = new Array(maxY).fill(true);
        }
        var colorGrid = new Array(numColors);
        for (var i = 0; i < numColors; i++) {
            colorGrid[i] = new Array(maxX);
            for (var j = 0; j < maxX; j++) {
                colorGrid[i][j] = new Array(maxY);
                for (var k = 0; k < maxY; k++) {
                    colorGrid[i][j][k] = (random(3) == 0); // each color covers around 1/3 of the board
                }
            }
        }
        var initShade = new Array(maxX);
        for (var i = 0; i < maxX; i++) {
            initShade[i] = new Array(maxY);
            for (var j = 0; j < maxY; j++) {
                initShade[i][j] = (random(2) == 1); // equal amounts of light and dark
            }
        }
        var button = new Array(maxX);
        var squares = [];
        for (var i = 0; i < maxX; i++) {
            button[i] = new Array(maxY);
            for (var j = 0; j < maxY; j++) {
                squares.push([i, j]);
            }
        }
        randomShuffle(squares);
        var target = squares[0];
        for (var i = 0; i < numColors; i++) {
            button[squares[i + 1][0]][squares[i + 1][1]] = i;
        }
        var inGrid = 0;
        for (var i = 0; i < maxX; i++) {
            for (var j = 0; j < maxY; j++) {
                inGrid += Number(grid[i][j]);
            }
        }
        var extra = random(Math.floor((inGrid - numColors - numBalls - 1) / 5));
        for (var i = 0; i < extra; i++) {
            button[squares[i + numColors + 1][0]][squares[i + numColors + 1][0]] = random(numColors);
        }
        return board(maxX, maxY, numColors, numBalls, grid, colorGrid, initShade, button, target);
    }

    randomState() {
        squares = [];
        for (var i = 0; i < this.maxX; i++) {
            for (var j = 0; j < this.maxY; j++) {
                if (!this.grid[i][j] || this.button[i][j] != -1 || pairEqual(this.target, [i, j]))
                    continue;
                squares.push_back([i, j]);
            }
        }
        randomShuffle(squares);
        if (squares.length < this.numBalls) {
            cout << "ERROR in random state generator: not enough space to place balls\n";
        }
        var ballPos = [];
        for (var i = 0; i < this.numBalls; i++) {
            ballPos.push(squares[i]);
        }
        var colorFlip = new Array(numColors).fill(false);
        return state(this, ballPos, colorFlip);
    }

    // return move data as well?
    hint(hash) {
        var d = this.distance[hash];
        if (d == -1) {
            return -1;
        }
        else if (d == 0) {
            return -2;
        }
        else {
            var bestMoves = [];
            for (var i = 0; i < 4 * this.numBalls; i++) {
                if (this.distance[this.adj[hash][i]] == d - 1) {
                    bestMoves.push(i);
                }
            }
            return bestMoves[random(bestMoves.length)];
        }
    }

    solve(hash) {
        var moves = [];
        if (this.distance[hash] == -1) {
            cout << "Impossible to solve from this state\n";
            return moves;
        }
        while (this.distance[hash] > 0) {
            var bestMove = this.hint(hash);
            moves.push(bestMove);
            hash = this.adj[hash][bestMove];
        }
        return moves;
    }
}