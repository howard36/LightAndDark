var canvas = document.getElementById("myCanvas");
var ctx = canvas.getContext("2d");

var grid = [
    [0, 0, 1, 1, 1],
    [0, 1, 1, 1, 1],
    [1, 1, 1, 1, 1],
    [1, 1, 1, 1, 0],
    [1, 1, 1, 0, 0]
];
var colorGrid = [
    [
        [0, 0, 0, 1, 0],
        [0, 0, 0, 0, 1],
        [0, 1, 0, 0, 1],
        [0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0]
    ],
    [
        [0, 0, 0, 1, 0],
        [0, 0, 1, 0, 0],
        [0, 0, 0, 1, 0],
        [1, 0, 0, 0, 0],
        [0, 1, 1, 0, 0]
    ]
];
var initShade = [
    [0, 0, 0, 1, 0],
    [0, 1, 0, 1, 0],
    [0, 1, 0, 1, 0],
    [0, 0, 1, 0, 0],
    [0, 1, 1, 0, 0]
];
var button = [
    [-1, -1, 0, -1, -1],
    [-1, -1, -1, -1, -1],
    [1, -1, -1, -1, 1],
    [-1, -1, -1, -1, -1],
    [-1, -1, 0, -1, -1]
];

for (var i = 0; i < 5; i++) {
    for (var j = 0; j < 5; j++) {
        grid[i][j] = Boolean(grid[i][j]);
        initShade[i][j] = Boolean(initShade[i][j]);
        for (var k = 0; k < 2; k++) {
            colorGrid[k][i][j] = Boolean(colorGrid[k][i][j]);
        }
    }
}
var target = [2, 2];

var ballPos = [[4, 0], [0, 4]];
var colorFlip = [false, false];

var aa = [1, 2, [3,4]];
var bb = JSON.parse(JSON.stringify(aa));
bb[2][0] = 100;
bb[1] = -100;
console.log(aa);
console.log(bb);
console.log(JSON.stringify(aa));
console.log(typeof(bb));

var sampleBoard = new Board(5, 5, 2, 2, grid, colorGrid, initShade, button, target);
var sampleState = new State(sampleBoard, ballPos, colorFlip);

// var s = JSON.parse(JSON.stringify(sampleState)); // copy current state
// s.ballPos[1] = [2,4];
// console.log(sampleState);
// console.log(s);

var sampleLevel = new Level(sampleState);
console.log(sampleState.applyMove(4));
console.log(sampleState);
sampleLevel.play(canvas);


canvas.addEventListener("click", function (event) { sampleLevel.canvasClick(event); });