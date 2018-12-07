function moveToString(move) {
    var s = "Move ball ";
    s += Math.floor(move / 4).toString();
    var dir = move % 4;
    if (dir == 0)
        s += " down";
    else if (dir == 1)
        s += " right";
    else if (dir == 2)
        s += " up";
    else
        s += " left";
    return s;
}

class Level {
    constructor(state) {
        this.initState = state;
        this.currentState = this.initState;
        this.currentHash = this.currentState.getHash();
        this.b = state.b;
        this.difficulty = this.b.getDistance()[this.currentHash];
        this.playing = false;
        this.selectedBall = -1;
        this.animation = false;

        // for display purposes
        this.squareSize = 80;
        this.buffer = 20;
    }

    selectBall(ball) {
        if (!this.playing) {
            console.log("Not currently playing");
            return;
        }
        if (this.animation) {
            console.log("Animation is currently in progress");
            return;
        }
        this.selectedBall = ball;
        console.log(`Ball ${ball} selected`);
        // TODO: start animation
    }

    unselectBall() {
        if (!this.playing) {
            console.log("Not currently playing");
            return;
        }
        if (this.animation) {
            console.log("Animation is currently in progress");
            return;
        }
        console.log(`Ball ${this.selectedBall} unselected`);
        this.selectedBall = -1;
        // TODO: start animation
    }

    move(dir) {
        if (!this.playing) {
            console.log("Not currently playing");
            return;
        }
        if (this.animation) {
            console.log("Animation is currently in progress");
            return;
        }
        if (this.selectedBall == -1) {
            console.log("No ball selected");
            return;
        }
        // console.log(`unhash(2016):`);
        // console.log(this.b.unhash(2016));
        var nextHash = this.b.move(this.currentHash, this.selectedBall, dir);
        if (nextHash == this.currentHash) {
            console.log(`Invalid Move (had no effect). You tried to move ball ${this.selectedBall} in direction ${dir}`);
            return;
        }
        console.log(`Moving ball ${this.selectedBall} in direction ${dir}`);
        // console.log(`Nexthash = ${nextHash}`);
        // console.log(`unhash(nextHash).gethash() = ${this.b.unhash(nextHash).getHash()}`);
        // console.log(`hash(unhash(nextHash)) = ${this.b.hash(this.b.unhash(nextHash))}`);
        // TODO: state transition animation from currentHash to nextHash
        this.unselectBall();
        document.getElementById("message").innerHTML = "";
        this.currentHash = nextHash;
        this.currentState = this.b.unhash(this.currentHash);
        this.currentState.display();
        this.history.push(this.currentHash);
        if (this.currentState.getWin()) {
            var message = document.getElementById("message");
            message.innerHTML = "You Won!";
            this.playing = false;
        }
    }

    play() {
        this.playing = true;
        this.currentState = this.initState;
        this.currentHash = this.initState.getHash();
        this.currentState.display();
        this.history = [this.initState.getHash()];
        document.getElementById("message").innerHTML = "";
    }

    canvasClick(event) {
        var canvas = document.getElementById("gameCanvas");
        var x = event.pageX - canvas.offsetLeft;
        var y = event.pageY - canvas.offsetTop;
        if (x < this.buffer || y < this.buffer || x >= this.b.maxY * this.squareSize + this.buffer || y >= this.b.maxX * this.squareSize + this.buffer) {
            console.log("Clicked outside of grid");
            return;
        }
        var gridX = Math.floor((y - this.buffer) / this.squareSize);
        var gridY = Math.floor((x - this.buffer) / this.squareSize);
        this.squareClick(gridX, gridY);
    }

    squareClick(x, y) {
        if (!this.playing) {
            console.log("Not currently playing");
            return;
        }
        console.log(`Clicked square (${x}, ${y})`);
        var ball = this.currentState.occupied([x, y]);
        if (this.selectedBall != -1) {
            if (ball == this.selectedBall) {
                this.unselectBall();
            }
            else {
                var dir = this.findDir(this.currentState.getBallPos()[this.selectedBall], [x, y]);
                if (dir != -1) {
                    // TODO: check if it changes state
                    this.move(dir);
                }
            }
        }
        else {
            this.selectBall(ball);
        }
    }

    // moving from p1 to p2
    findDir(p1, p2) {
        if (p2[0] > p1[0] && p2[1] == p1[1])
            return 0;
        else if (p2[0] == p1[0] && p2[1] > p1[1])
            return 1;
        else if (p2[0] < p1[0] && p2[1] == p1[1])
            return 2;
        else if (p2[0] == p1[0] && p2[1] < p1[1])
            return 3;
        else
            return -1;
    }

    hint() {
        if (!this.playing) {
            console.log("Not currently playing");
            return;
        }
        var message = document.getElementById("message");
        var hint = this.b.hint(this.currentHash);
        if (hint == -1) {
            message.innerHTML = "The current state is impossible to solve, you should restart";
        }
        else if (hint == -2) {
            console.log("Error: hint returned winning position");
        }
        else {
            message.innerHTML = moveToString(hint);
        }
    }

    undo() {
        if (!this.playing) {
            console.log("Not currently playing");
            return;
        }
        if (this.history.length == 1) {
            document.getElementById("message").innerHTML = "No moves to undo";
            return;
        }
        document.getElementById("message").innerHTML = "You undid your last move";
        this.history.pop();
        this.currentHash = this.history[this.history.length - 1];
        this.currentState = this.b.unhash(this.currentHash);
        this.currentState.display();
    }

    restart() {
        this.play();
    }
}