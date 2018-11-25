import java.util.Scanner;

public class Board {

    public Board(int numRows, int numCols, int numBalls, int numColours, int[][] initShade, boolean[][][] colourGrid,
            int[][] buttonGrid, Cell target) {
        this.numRows = numRows;
        this.numCols = numCols;
        this.numBalls = numBalls;
        this.numColours = numColours;
        this.initShade = initShade.clone();
        this.colourGrid = colourGrid.clone();
        this.buttonGrid = buttonGrid.clone();
        this.target = new Cell(target);
        state = null;
        initializeGridPanel();
    }

    public void play(Cell[] ballPositions) {
        state = new State(this, ballPositions);
        state.draw(gridPanel);
        gridPanel.setPlaying(true);
    }

    public void move(int command) {
        state = state.move(command / 4, command % 4);
        state.draw(gridPanel);
    }

    // TODO: Randomly generate initShade, colourGrid, buttonGrid, target
    public Board(int numRows, int numCols, int numBalls, int numColours) {
        this.numRows = numRows;
        this.numCols = numCols;
        this.numBalls = numBalls;
        this.numColours = numColours;
        initializeGridPanel();
    }

    public int getNumRows() {
        return numRows;
    }

    public int getNumCols() {
        return numCols;
    }

    public int getNumBalls() {
        return numBalls;
    }

    public int getNumColours() {
        return numColours;
    }

    public Cell getTarget() {
        return new Cell(target);
    }

    public int getInitShade(Cell cell) {
        return initShade[cell.row][cell.col];
    }

    public boolean getColour(int colour, Cell cell) {
        return colourGrid[colour][cell.row][cell.col];
    }

    public int getButton(Cell cell) {
        return buttonGrid[cell.row][cell.col];
    }

    private void initializeGridPanel() {
        gridPanel = new GridPanel(this);
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                Cell cell = new Cell(i, j);
                gridPanel.setColour(cell, initShade[i][j]);
                for (int c = 0; c < numColours; ++c) {
                    if (colourGrid[c][i][j]) {
                        gridPanel.addCircle(cell, c);
                    }
                }
                if (buttonGrid[i][j] != -1)
                    gridPanel.addCenterCircle(new Cell(i, j), buttonGrid[i][j]);
            }
        }
        gridPanel.blackout(target);
        gridPanel.redraw();
    }

    private int numRows;
    private int numCols;
    private int numColours;
    private int numBalls;
    private int[][] initShade;
    private boolean[][][] colourGrid;
    private int[][] buttonGrid;
    private Cell target;

    private State state;
    private GridPanel gridPanel;
}