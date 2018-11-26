import java.awt.event.*;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Stack;

public class GamePlayer implements KeyListener {

    public GamePlayer(Board board) {
        this.board = board;
        gridPanel = new GridPanel(board.getNumRows(), board.getNumCols());
        interactive = false;
        gridPanel.addKeyListener(this);
        board.draw(gridPanel);
        gridPanel.repaint();
        board.analyzeGraph();
    }

    public void playOptimally(State s) {
        interactive = false;
        setupPlay(s);
        Timer timer = new Timer();
        TimerTask task = new TimerTask() {
            public void run() {
                moveOptimally();
            }
        };
        timer.scheduleAtFixedRate(task, 2000, 2000);
    }

    public void playInteractively(State s) {
        System.out.printf("Type 0 to 3 to move ball 0, 4 to 7 to move ball 1\n");
        System.out.printf("Type h for a hint\n");
        System.out.printf("Type u to undo move\n");
        System.out.printf("Optimal solution uses %d steps\n", s.getDist());
        setupPlay(s);
        interactive = true;
    }

    public void playInteractivelyHardest() {
        playInteractively(board.getHardestState());
    }

    // methods of KeyListener
    public void keyPressed(KeyEvent ke) {
        if (!interactive)
            return;
        int code = ke.getKeyCode();
        if (48 <= code && code < 56) {
            int command = code - 48;
            move(command / 4, command % 4);
        } else if (code == 72) {
            // h for hint
            moveOptimally();
        } else if (code == 85) {
            // u for undo
            if (path.size() > 1) {
                path.pop();
                updateState();
            }
        }
    }

    public void keyReleased(KeyEvent ke) {
    }

    public void keyTyped(KeyEvent ke) {
    }

    private void setupPlay(State s) {
        path = new Stack<State>();
        path.push(board.getState(s));
        updateState();
    }

    private void move(int ball, int dir) {
        checkForWin();
        State state = path.peek().next(ball, dir);
        if (state != path.peek()) {
            path.push(state);
            updateState();
        }
    }

    private void moveOptimally() {
        checkForWin();
        path.push(path.peek().optimalNext());
        updateState();
    }

    private void updateState() {
        path.peek().draw(gridPanel);
        gridPanel.repaint();
    }

    private void checkForWin() {
        if (path.peek().getWin()) {
            System.out.printf("You won in %d steps\n", path.size() - 1);
            System.exit(0);
        }
    }

    private Board board;
    private GridPanel gridPanel;
    private boolean interactive;
    Stack<State> path;
}