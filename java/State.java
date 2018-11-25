import java.util.Arrays;

public class State {

    public State(Board board, Cell[] ballPositions) {
        this.board = board;
        this.ballPositions = ballPositions.clone();
        colourFlip = new int[board.getNumColours()];
        Arrays.fill(colourFlip, 0);
        win = computeWin();
    }

    public State(Board board, Cell[] ballPositions, int[] colourFlip) {
        this.board = board;
        this.ballPositions = ballPositions.clone();
        this.colourFlip = colourFlip.clone();
        win = computeWin();
    }

    public State(State s) {
        board = s.board;
        win = s.win;
        colourFlip = s.colourFlip.clone();
        ballPositions = s.ballPositions.clone();
    }

    public void draw(GridPanel gridPanel) {
        gridPanel.eraseX();
        for (int i = 0; i < ballPositions.length; ++i) {
            gridPanel.addX(ballPositions[i]);
        }
        for (int i = 0; i < board.getNumRows(); ++i) {
            for (int j = 0; j < board.getNumCols(); ++j) {
                Cell cell = new Cell(i, j);
                gridPanel.setColour(cell, getShade(cell));
            }
        }
        gridPanel.repaint();
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
        } else if (nextPos.row < 0 || nextPos.row >= board.getNumRows() || nextPos.col < 0
                || nextPos.col >= board.getNumCols()) {
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

    public boolean getWin() {
        return win;
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

    private Board board;
    private int[] colourFlip;
    private Cell[] ballPositions;
    private boolean win;
}