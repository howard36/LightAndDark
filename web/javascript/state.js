function pairEqual(a, b) {
    return a[0] == b[0] && a[1] == b[1];
}

function next(pos, dir) {
    var x = pos[0], y = pos[1];
    if (dir == 0)
        x++;
    else if (dir == 1)
        y++;
    else if (dir == 2)
        x--;
    else
        y--;
    return [x, y];
}

class State {
    constructor(b, ballPos, colorFlip) {
        this.b = b;
        this.ballPos = ballPos;
        this.colorFlip = colorFlip;
        this.hash = b.hash(this);
        this.win = b.checkWin(this);
    }

    getBallPos() { return JSON.parse(JSON.stringify(this.ballPos)) }; // change to position of single ball
    getColorFlip() { return JSON.parse(JSON.stringify(this.colorFlip)); }
    getWin() { return this.win; }
    getHash() { return this.hash; }

    static copy(state) {
        // board does not need to be copied, because a state should not change it's board
        // if we copy board here, it leads to an infinite copying loop
        var ballPos = JSON.parse(JSON.stringify(state.ballPos));
        var colorFlip = JSON.parse(JSON.stringify(state.colorFlip));
        return new State(state.b, ballPos, colorFlip);
    }

    inGrid(pos) {
        var x = pos[0], y = pos[1];
        return (x >= 0 && y >= 0 && x < this.b.maxX && y < this.b.maxY && this.b.grid[x][y]);
    }

    getShade(pos) {
        var x = pos[0], y = pos[1];
        var shade = this.b.initShade[x][y];
        for (var i = 0; i < this.b.numColors; i++) {
            if (this.b.colorGrid[i][x][y] && this.colorFlip[i]) {
                shade = !shade;
            }
        }
        return shade;
    }

    occupied(pos) {
        for (var i = 0; i < this.ballPos.length; i++) {
            if (pairEqual(this.ballPos[i], pos))
                return i;
        }
        return -1;
    }

    isButton(pos) {
        return this.b.button[pos[0]][pos[1]];
    }

    // dir = 0: down (positive x direction)
    // dir = 1: left (positive y direction)
    // dir = 2: up (negative x direction)
    // dir = 3: right (negative y direction)
    rollBall(ball, dir) {
        if (ball < 0 || ball > this.ballPos.length) {
            console.log("Error: invalid ball");
        }
        var pos = JSON.parse(JSON.stringify(this.ballPos[ball]));
        if (pairEqual(pos, this.b.target))
            return pos;
        for (var i = 0; i < Math.max(this.b.maxX, this.b.maxY); i++) { // check if upper bound is correct
            // check if ball stops after moving i squares in direction dir, or continues moving
            var nextPos = next(pos, dir);
            if (pairEqual(nextPos, this.b.target))
                return nextPos;
            if (!this.inGrid(nextPos))
                return pos; // hits outer wall, cannot move
            if (this.getShade(pos) != this.getShade(nextPos))
                return pos; // next position is different shade
            if (this.occupied(nextPos) != -1)
                return pos; // next position contains another ball
            if (this.isButton(nextPos) != -1)
                return nextPos;
            // ball continues moving
            pos = JSON.parse(JSON.stringify(nextPos)); // is this necessary?
        }
        console.log("ERROR in state::move: ball did not stop after max(maxX, maxY) moves\n");
        return [-1, -1];
    }

    applyMove(move) {
        var ball = Math.floor(move / 4), dir = move % 4;
        var newPos = this.rollBall(ball, dir);
        if (pairEqual(newPos, this.ballPos[ball])) {
            return [this.hash, [this.ballPos[ball], newPos]];
        }
        else {
            var s = State.copy(this);
            s.ballPos[ball] = newPos;
            var buttonColor = s.isButton(newPos);
            if (buttonColor != -1) {
                s.colorFlip[buttonColor] = !s.colorFlip[buttonColor];
            }
            return [s.b.hash(s), [this.ballPos[ball], newPos]];
        }
    }

    hint() {
        return this.b.hint(this.hash);
    }

    solve() {
        return this.b.solve(this.hash);
    }

    displaySquare(pos, ctx) {
        var ballColors = ["#6ab43e", "#00a4ef", "#f98026"];
        var buttonColors = ["#9c41f4", "#e10d0d", "#fee400"];
        var x = pos[0], y = pos[1];
        if (this.b.grid[x][y] == 0) {
            return;
        }
        var squareSize = 80;
        var width = 8;
        var ballRadius = 25;
        var buttonRadius = 30;
        var buttonWidth = 6;
        var iconRadius = 5;
        var iconCornerShift = 8 + width / 2;
        var buffer = 20;

        ctx.beginPath();
        ctx.strokeStyle = "#0095DD";
        ctx.lineWidth = width;
        if (pairEqual(pos, this.b.target)) {
            ctx.fillStyle = "#fff";
        }
        else if (this.getShade(pos) == 1) {
            ctx.fillStyle = "#222";
        }
        else {
            ctx.fillStyle = "#ccc";
        }
        ctx.rect(y * squareSize + buffer, x * squareSize + buffer, squareSize, squareSize);
        ctx.fill();
        ctx.stroke();
        if (this.occupied(pos) != -1) {
            // console.log(`Contains ball ${this.occupied(pos)}`);
            ctx.beginPath();
            ctx.arc(y * squareSize + squareSize / 2 + buffer, x * squareSize + squareSize / 2 + buffer, ballRadius, 0, 2 * Math.PI);
            ctx.fillStyle = ballColors[this.occupied(pos)];
            ctx.fill();
        }
        if (this.isButton(pos) != -1) {
            // console.log(`Contains button ${this.isButton(pos)}`);
            ctx.beginPath();
            ctx.arc(y * squareSize + squareSize / 2 + buffer, x * squareSize + squareSize / 2 + buffer, buttonRadius, 0, 2 * Math.PI);
            ctx.strokeStyle = buttonColors[this.isButton(pos)];
            ctx.lineWidth = buttonWidth;
            ctx.stroke();
        }
        if (this.b.numColors > 0 && this.b.colorGrid[0][x][y]) {
            // console.log("Color 0");
            ctx.beginPath();
            ctx.arc(y * squareSize + buffer + iconCornerShift, x * squareSize + iconCornerShift + buffer, iconRadius, 0, 2 * Math.PI);
            ctx.fillStyle = buttonColors[0];
            ctx.fill();
        }
        if (this.b.numColors > 1 && this.b.colorGrid[1][x][y]) {
            // console.log("Color 1");
            ctx.beginPath();
            ctx.arc((y + 1) * squareSize + buffer - iconCornerShift, x * squareSize + iconCornerShift + buffer, iconRadius, 0, 2 * Math.PI);
            ctx.fillStyle = buttonColors[1];
            ctx.fill();
        }
        if (this.b.numColors > 2 && this.b.colorGrid[2][x][y]) {
            ctx.beginPath();
            ctx.arc(y * squareSize + buffer + iconCornerShift, (x + 1) * squareSize - iconCornerShift + buffer, iconRadius, 0, 2 * Math.PI);
            ctx.fillStyle = buttonColors[2];
            ctx.fill();
        }
    }

    display() {
        var canvas = document.getElementById("gameCanvas");
        var ctx = canvas.getContext("2d");
        console.log("Displaying State");
        console.log(this);
        var shades = new Array(this.b.maxX);
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        for (var i = 0; i < this.b.maxX; i++) {
            shades[i] = new Array(this.b.maxY);
            for (var j = 0; j < this.b.maxY; j++) {
                // console.log(`Displaying (${i}, ${j})`);
                this.displaySquare([i, j], ctx);
                shades[i][j] = this.getShade([i, j]);
            }
        }
        console.log(shades);
    }
}