import java.util.Queue;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;
import java.util.Random;
import java.util.ArrayList;

public class Board {

    public Board(int numRows, int numCols, int numBalls, int numColours, int[][] initShade, boolean[][][] colourGrid,
            int[][] buttonGrid, boolean[][] grid, Cell target) {
        this.numRows = numRows;
        this.numCols = numCols;
        this.numBalls = numBalls;
        this.numColours = numColours;
        this.initShade = initShade.clone();
        this.colourGrid = colourGrid.clone();
        this.buttonGrid = buttonGrid.clone();
        this.grid = grid.clone();
        this.target = new Cell(target);
        buildGraph();
    }

    public Board(int numRows, int numCols, int numBalls, int numColours) {
        Random rand = new Random();

        this.numRows = numRows;
        this.numCols = numCols;
        this.numBalls = numBalls;
        this.numColours = numColours;
        initShade = new int[numRows][numCols];
        colourGrid = new boolean[numColours][numRows][numCols];
        buttonGrid = new int[numRows][numCols];
        grid = new boolean[numRows][numCols];
        for (int i = 0; i < numRows; ++i)
            for (int j = 0; j < numCols; ++j)
                grid[i][j] = rand.nextInt(7) != 0;
        for (int c = 0; c < numColours; ++c)
            for (int i = 0; i < numRows; ++i)
                for (int j = 0; j < numCols; ++j)
                    colourGrid[c][i][j] = rand.nextInt(3) == 0;
        List<Cell> cells = new ArrayList<Cell>();
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                initShade[i][j] = rand.nextInt(2);
                buttonGrid[i][j] = -1;
                if (grid[i][j])
                    cells.add(new Cell(i, j));
            }
        }
        Collections.shuffle(cells);
        target = cells.get(0);
        for (int i = 0; i < numColours; ++i)
            buttonGrid[cells.get(i + 1).row][cells.get(i + 1).col] = i;
        int extra = rand.nextInt((cells.size() - numColours - numBalls - 1) / 4);
        for (int i = 0; i < extra; ++i)
            buttonGrid[cells.get(1 + numColours + i).row][cells.get(1 + numColours + i).col] = rand.nextInt(numColours);
        buildGraph();
    }

    public void analyzeGraph() {
        Queue<State> q = new LinkedList<State>();
        for (int i = 0; i < allStates.length; ++i) {
            if (allStates[i].getValid() && allStates[i].getWin()) {
                allStates[i].setDist(0);
                q.add(allStates[i]);
            }
        }
        while (!q.isEmpty()) {
            State u = q.poll();
            List<State> in = u.getIn();
            for (State v : in) {
                if (v.getDist() == -1) {
                    v.setDist(1 + u.getDist());
                    q.add(v);
                }
            }
        }
    }

    public State getHardestState() {
        State best = allStates[0];
        for (State state : allStates)
            if (state.getDist() > best.getDist())
                best = state;
        return best;
    }

    public void draw(GridPanel gridPanel) {
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                Cell cell = new Cell(i, j);
                if (grid[i][j])
                    gridPanel.setColour(cell, initShade[i][j]);
                else
                    gridPanel.fadeOut(cell);
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
    }

    public State getState(State state) {
        return allStates[hash(state)];
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

    public boolean validCell(Cell cell) {
        return cell.row >= 0 && cell.row < numRows && cell.col >= 0 && cell.col < numCols && grid[cell.row][cell.col];
    }

    private void buildGraph() {
        int h = 1 << numColours;
        for (int i = 0; i < numBalls; ++i)
            h *= numRows * numCols;
        allStates = new State[h];
        for (int i = 0; i < h; ++i) {
            allStates[i] = unhash(i);
        }
        for (State s : allStates) {
            if (s.getValid()) {
                for (int ball = 0; ball < numBalls; ++ball) {
                    for (int dir = 0; dir < 4; ++dir) {
                        State next = allStates[hash(s.move(ball, dir))];
                        s.addOut(ball, dir, next);
                        next.addIn(s);
                    }
                }
            }
        }
    }

    private int hash(State s) {
        int h = 0;
        for (int i = 0; i < numBalls; i++) {
            h = numCols * h + s.getBallPos(i).col;
            h = numRows * h + s.getBallPos(i).row;
        }
        for (int i = 0; i < numColours; i++) {
            h = 2 * h + s.getColourFlip(i);
        }
        return h;
    }

    private State unhash(int h) {
        Cell[] ballPositions = new Cell[numBalls];
        for (int i = 0; i < numBalls; ++i)
            ballPositions[i] = new Cell();
        int[] colourFlip = new int[numColours];
        for (int i = numColours - 1; i >= 0; i--) {
            colourFlip[i] = (h % 2);
            h /= 2;
        }
        for (int i = numBalls - 1; i >= 0; i--) {
            ballPositions[i].row = h % numRows;
            h /= numRows;
            ballPositions[i].col = h % numCols;
            h /= numCols;
        }
        State s = new State(this, ballPositions, colourFlip);
        return s;
    }

    private final int numRows;
    private final int numCols;
    private final int numColours;
    private final int numBalls;
    private int[][] initShade;
    private boolean[][][] colourGrid;
    private int[][] buttonGrid;
    private boolean[][] grid;
    private Cell target;

    State[] allStates;
}