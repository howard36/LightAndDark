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
        console.log(`Nexthash = ${nextHash}`);
        console.log(`unhash(nextHash).gethash() = ${this.b.unhash(nextHash).getHash()}`);
        console.log(`hash(unhash(nextHash)) = ${this.b.hash(this.b.unhash(nextHash))}`);
        // TODO: state transition animation from currentHash to nextHash
        this.unselectBall();
        this.currentHash = nextHash;
        this.currentState = this.b.unhash(this.currentHash);
        this.currentState.display(this.canvas);
    }

    play(canvas) {
        this.playing = true;
        this.currentState = this.initState;
        this.currentHash = this.initState.getHash();
        this.canvas = canvas;
        this.currentState.display(this.canvas);
    }

    canvasClick(event) {
        var x = event.pageX - this.canvas.offsetLeft;
        var y = event.pageY - this.canvas.offsetTop;
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
}