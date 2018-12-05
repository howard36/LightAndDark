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
    }

    displaySquare(x, y, ctx) {
        if (this.grid[x][y] == 0) {
            return;
        }
        var squareSize = 80;
        var squareGap = 0;
        var width = 8;
        ctx.beginPath();
        ctx.strokeStyle = "#0095DD";
        ctx.lineWidth = width;
        // console.log(`grid[${x}][${y}] = ${this.grid[x][y]}`);
        if (this.initShade[x][y] == 1) {
            ctx.fillStyle = "#222";
        }
        else {
            ctx.fillStyle = "#ddd";
        }
        ctx.rect(y*squareSize + (y+1)*squareGap + width, x*squareSize + (x+1)*squareGap + width, squareSize, squareSize);
        ctx.fill();
        ctx.stroke();
    }

    display(ctx) {
        console.log("Displaying Board");
        for (var i = 0; i < this.maxX; i++) {
            for (var j = 0; j < this.maxY; j++) {
                this.displaySquare(i, j, ctx);
            }
        }
    }
}