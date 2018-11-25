import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;

class GridPanel extends JPanel {

    public GridPanel(int numRows, int numCols) {
        JFrame frame = new JFrame();
        frame.setSize(800, 800);
        frame.setLocation(100, 100);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLayout(new BorderLayout());
        frame.add(this);
        frame.setVisible(true);

        this.numRows = numRows;
        this.numCols = numCols;
        bg = new int[numRows][numCols];
        circles = new int[numRows][numCols][4];
        centerCircles = new int[numRows][numCols];
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                bg[i][j] = 0;
                centerCircles[i][j] = -1;
                for (int k = 0; k < 4; ++k)
                    circles[i][j][k] = -1;
            }
        }
        crossPositions = new ArrayList<Cell>();

        setFocusable(true);
    }

    public void paint(Graphics g) {
        Dimension dim = getSize();
        int len = Math.min(dim.width / numCols, dim.height / numRows);
        int offsetx = (dim.width - len * numCols) / 2;
        int offsety = (dim.height - len * numRows) / 2;
        for (int row = 0; row < numRows; ++row) {
            for (int col = 0; col < numCols; ++col) {
                int x = col * len + offsetx;
                int y = row * len + offsety;
                g.setColor(backgroundColours[bg[row][col]]);
                g.fillRect(x, y, len, len);
                g.setColor(borderColour);
                g.drawRect(x, y, len, len);
                for (int i = 0; i < circles[row][col].length && circles[row][col][i] != -1; ++i) {
                    g.setColor(accentColours[circles[row][col][i]]);
                    g.fillOval(x, y, len / 5, len / 5);
                    if (i == 0)
                        x += 2 * len / 3;
                    else if (i == 1)
                        y += 2 * len / 3;
                    else if (i == 2)
                        x -= 2 * len / 3;
                    else
                        y -= 2 * len / 3;
                }
                if (centerCircles[row][col] != -1) {
                    x = col * len + offsetx;
                    y = row * len + offsety;
                    g.setColor(accentColours[centerCircles[row][col]]);
                    // radius is len/2
                    g.fillOval(x + len / 3, y + len / 3, len / 3, len / 3);
                }
            }
        }
        for (int i = 0; i < crossPositions.size(); ++i) {
            g.setColor(crossColours[i]);
            int x = crossPositions.get(i).col * len + offsetx;
            int y = crossPositions.get(i).row * len + offsety;
            g.drawLine(x, y, x + len, y + len);
            g.drawLine(x + len, y, x, y + len);
        }
    }

    public void setColour(Cell c, int colour) {
        if (bg[c.row][c.col] != 2)
            bg[c.row][c.col] = colour;
    }

    public void addCircle(Cell c, int circleColour) {
        for (int i = 0; i < 4; ++i) {
            if (circles[c.row][c.col][i] == -1) {
                circles[c.row][c.col][i] = circleColour;
                return;
            }
        }
    }

    public void blackout(Cell c) {
        bg[c.row][c.col] = 2;
    }

    public void eraseX() {
        crossPositions.clear();
    }

    public void addX(Cell c) {
        crossPositions.add(new Cell(c));
    }

    public void addCenterCircle(Cell c, int colour) {
        centerCircles[c.row][c.col] = colour;
    }

    public void reset() {
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                bg[i][j] = 0;
                centerCircles[i][j] = -1;
                for (int k = 0; k < 4; ++k)
                    circles[i][j][k] = -1;
            }
        }
        crossPositions.clear();
        repaint();
    }

    private static final long serialVersionUID = 69L;
    private Board board;
    private int numRows;
    private int numCols;

    private static final Color[] backgroundColours = { Color.white, Color.gray, Color.black };
    private static final Color[] accentColours = { Color.red, Color.blue, Color.green };
    private static final Color borderColour = Color.magenta;
    private static final Color[] crossColours = { Color.black, Color.green };

    private int[][] bg;
    private int[][][] circles;
    private int[][] centerCircles;
    private ArrayList<Cell> crossPositions;
}