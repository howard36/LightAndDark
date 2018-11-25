import java.awt.event.*;

public class GamePlayer implements KeyListener {
    public GamePlayer(Board board, State state) {
        this.board = board;
        this.state = state;
        gridPanel = new GridPanel(board.getNumRows(), board.getNumCols());
        gridPanel.addKeyListener(this);
    }

    public void playInteractively() {
        board.draw(gridPanel);
        state.draw(gridPanel);
        gridPanel.repaint();
    }

    // methods of KeyListener
    public void keyPressed(KeyEvent ke) {
        int code = ke.getKeyCode();
        System.out.printf("key pressed with code %d\n", code);
        if (48 <= code && code < 56) {
            int command = code - 48;
            move(command / 4, command % 4);
        }
    }

    public void keyReleased(KeyEvent ke) {
    }

    public void keyTyped(KeyEvent ke) {
    }

    private void move(int ball, int dir) {
        state = state.move(ball, dir);
        state.draw(gridPanel);
        gridPanel.repaint();
        if (state.getWin()) {
            System.out.println("You Won!");
            System.exit(0);
        }
    }

    private Board board;
    private State state;
    private GridPanel gridPanel;
}