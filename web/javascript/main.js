var canvas = document.getElementById("myCanvas");
var ctx = canvas.getContext("2d");

var grid = [
    [0, 0, 1, 1, 1],
    [0, 1, 1, 1, 1],
    [1, 1, 1, 1, 1],
    [1, 1, 1, 1, 0],
    [1, 1, 1, 0, 0]
];
var initShade = [
    [0, 0, 0, 1, 0],
    [0, 1, 0, 1, 0],
    [0, 1, 0, 1, 0],
    [0, 0, 1, 0, 0],
    [0, 1, 1, 0, 0]
];

var b = new Board(5, 5, 0, 0, grid, 0, initShade, 0, 0);
b.display(ctx);
