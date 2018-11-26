import java.awt.event.KeyListener;
import java.awt.event.KeyEvent;
import java.util.Timer;
import java.util.TimerTask;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Stack;

public class GamePlayer implements KeyListener {

    public GamePlayer() {
        interactive = false;
    }

    public void play() {
        System.out.print("Enter 0 to play and 1 to watch: ");
        try {
            BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
            int mode = Integer.parseInt(in.readLine());
            if (mode == 0) {
                playRandomInteractively();
            } else {
                playRandomOptimally();
            }
        } catch (IOException e) {
            playRandomInteractively();
        }
    }

    public void playOptimally(Board board, State s) {
        interactive = false;
        board.analyzeGraph();
        setupPlay(board, s);
        timer = new Timer();
        TimerTask task = new TimerTask() {
            public void run() {
                moveOptimally();
            }
        };
        timer.scheduleAtFixedRate(task, 2000, 2000);
    }

    public void playInteractively(Board board, State s) {
        interactive = true;
        board.analyzeGraph();
        System.out.printf("Type 0 to 3 to move ball 0, 4 to 7 to move ball 1\n");
        System.out.printf("Type h for a hint\n");
        System.out.printf("Type u to undo move\n");
        System.out.printf("Optimal solution uses %d steps\n", s.getDist());
        setupPlay(board, s);
    }

    public void playHardestInteractively(Board board) {
        board.analyzeGraph();
        playInteractively(board, board.getHardestState());
    }

    public void playHardestOptimally(Board board) {
        board.analyzeGraph();
        playOptimally(board, board.getHardestState());
    }

    public void playRandomInteractively() {
        playHardestInteractively(getRandomBoard());
    }

    public void playRandomOptimally() {
        playHardestOptimally(getRandomBoard());
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

    private void setupPlay(Board board, State s) {
        gridPanel = new GridPanel(board.getNumRows(), board.getNumCols());
        gridPanel.addKeyListener(this);
        board.draw(gridPanel);
        gridPanel.repaint();
        path = new Stack<State>();
        path.push(board.getState(s));
        updateState();
    }

    private void move(int ball, int dir) {
        if (path.peek().getWin()) {
            System.out.printf("You won in %d steps\n", path.size() - 1);
            play();
            return;
        }
        State state = path.peek().next(ball, dir);
        if (state != path.peek()) {
            path.push(state);
            updateState();
        } else {
            java.awt.Toolkit.getDefaultToolkit().beep();
        }
    }

    private void moveOptimally() {
        if (path.peek().getWin()) {
            System.out.printf("Finished optimal play.\n");
            if (!interactive)
                timer.cancel();
            play();
            return;
        }
        path.push(path.peek().optimalNext());
        updateState();
    }

    private void updateState() {
        path.peek().draw(gridPanel);
        gridPanel.repaint();
    }

    private Board getRandomBoard() {
        try {
            BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
            System.out.print("Enter the number of rows: ");
            int numRows = Integer.parseInt(in.readLine());
            System.out.print("Enter the number of cols: ");
            int numCols = Integer.parseInt(in.readLine());
            System.out.print("Enter the number of balls: ");
            int numBalls = Integer.parseInt(in.readLine());
            System.out.print("Enter the number of colours: ");
            int numColours = Integer.parseInt(in.readLine());
            return new Board(numRows, numCols, numBalls, numColours);
        } catch (IOException e) {
            return new Board(5, 5, 2, 2);
        }
    }

    private GridPanel gridPanel;
    private boolean interactive;
    private Stack<State> path;
    private Timer timer;
}