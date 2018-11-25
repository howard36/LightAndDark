import java.util.Arrays;
import java.util.Collections;
import java.util.ArrayList;
import java.util.List;

public class State {

    public State(Board board, Cell[] ballPositions, int[] colourFlip) {
        this.board = board;
        this.ballPositions = ballPositions.clone();
        this.colourFlip = colourFlip.clone();
        win = computeWin();
        valid = computeValid();
        out = new State[4 * ballPositions.length];
        in = new ArrayList<State>();
        dist = -1;
    }

    // Default to no colour flips
    public State(Board board, Cell[] ballPositions) {
        this(board, ballPositions, new int[board.getNumColours()]);
    }

    public State(State s) {
        board = s.board;
        win = s.win;
        colourFlip = s.colourFlip.clone();
        ballPositions = s.ballPositions.clone();
        out = new State[4 * ballPositions.length];
        in = new ArrayList<State>();
        dist = s.dist;
    }

    public void draw(GridPanel gridPanel) {
        gridPanel.eraseX();
        for (int i = 0; i < ballPositions.length; ++i) {
            gridPanel.addX(ballPositions[i]);
        }
        for (int i = 0; i < board.getNumRows(); ++i) {
            for (int j = 0; j < board.getNumCols(); ++j) {
                Cell cell = new Cell(i, j);
                if (board.validCell(cell))
                    gridPanel.setColour(cell, getShade(cell));
            }
        }
    }

    public State next(int ball, int dir) {
        return out[4 * ball + dir];
    }

    public State move(int ball, int dir) {
        if (win)
            return this;
        Cell pos = ballPositions[ball];
        if (pos.equals(board.getTarget()))
            return this;
        int shade = getShade(pos);
        Cell nextPos = new Cell(pos.row, pos.col);
        if (dir == 0) {
            ++nextPos.col;
        } else if (dir == 1) {
            ++nextPos.row;
        } else if (dir == 2) {
            --nextPos.col;
        } else {
            --nextPos.row;
        }
        if (nextPos.equals(board.getTarget())) {
            State s = new State(this);
            s.moveBall(ball, nextPos);
            return s;
        } else if (!board.validCell(nextPos)) {
            return this;
        } else if (shade != getShade(nextPos)) {
            return this;
        } else if (occupied(nextPos)) {
            return this;
        } else if (board.getButton(nextPos) != -1) {
            State s = new State(this);
            s.moveBall(ball, nextPos);
            s.flip(board.getButton(nextPos));
            return s;
        } else {
            State s = new State(this);
            s.moveBall(ball, nextPos);
            return s.move(ball, dir);
        }
    }

    public State optimalNext() {
        if (dist == 0)
            return this;
        for (State next : out) {
            if (next.getDist() == dist - 1) {
                return next;
            }
        }
        return this;
    }

    public int getDist() {
        return dist;
    }

    public void setDist(int dist) {
        this.dist = dist;
    }

    public boolean getWin() {
        return win;
    }

    public boolean getValid() {
        return valid;
    }

    public Cell getBallPos(int ball) {
        return new Cell(ballPositions[ball]);
    }

    public int getColourFlip(int colour) {
        return colourFlip[colour];
    }

    public List<State> getIn() {
        // You cannot modify the elements in the list
        // But you can call methods on elements in the list
        // Which will then modify them for you.
        return Collections.unmodifiableList(in);
    }

    public void addOut(int ball, int dir, State next) {
        out[ball * 4 + dir] = next;
    }

    public void addIn(State pre) {
        in.add(pre);
    }

    private int getShade(Cell c) {
        int shade = board.getInitShade(c);
        for (int i = 0; i < board.getNumColours(); ++i) {
            if (board.getColour(i, c)) {
                shade ^= colourFlip[i];
            }
        }
        return shade;
    }

    private boolean occupied(Cell c) {
        for (int i = 0; i < ballPositions.length; ++i) {
            if (c.equals(ballPositions[i])) {
                return true;
            }
        }
        return false;
    }

    private void moveBall(int ball, Cell pos) {
        ballPositions[ball] = new Cell(pos);
        win = computeWin();
    }

    private void flip(int colour) {
        colourFlip[colour] ^= 1;
    }

    private boolean computeWin() {
        for (int i = 0; i < ballPositions.length; ++i) {
            if (!ballPositions[i].equals(board.getTarget())) {
                return false;
            }
        }
        return true;
    }

    private boolean computeValid() {
        if (ballPositions.length != board.getNumBalls())
            return false;
        if (colourFlip.length != board.getNumColours())
            return false;
        for (int i = 0; i < ballPositions.length; i++) {
            Cell p = ballPositions[i];
            if (p.row < 0 || p.row >= board.getNumRows() || p.col < 0 || p.col >= board.getNumCols())
                return false;
            // if (grid[p.x][p.y] == 0)
            // return false;
        }
        for (int i = 0; i < ballPositions.length; i++) {
            for (int j = i + 1; j < ballPositions.length; j++) {
                if (ballPositions[i].equals(ballPositions[j]) && !ballPositions[i].equals(board.getTarget()))
                    return false;
            }
        }
        return true;
    }

    private Board board;
    private int[] colourFlip;
    private Cell[] ballPositions;
    private boolean win;
    private boolean valid;

    private int dist;
    private State[] out;
    private List<State> in;
}