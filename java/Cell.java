public class Cell implements Cloneable {
    public int row;
    public int col;

    public Cell(int row, int col) {
        this.row = row;
        this.col = col;
    }

    public Cell() {
        this.row = 0;
        this.col = 0;
    }

    public Cell(Cell o) {
        this.row = o.row;
        this.col = o.col;
    }

    public boolean equals(Object o) {
        return (o instanceof Cell) && (row == ((Cell) o).row) && (col == ((Cell) o).col);
    }

    public Cell clone() throws CloneNotSupportedException {
        return (Cell) super.clone();
    }
};