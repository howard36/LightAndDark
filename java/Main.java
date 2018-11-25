public class Main {
    public static void main(String[] args) {
        System.out.println("In Main\n");
        boolean[][] grid = { { false, false, true, true, true }, { false, true, true, true, true },
                { true, true, true, true, true }, { true, true, true, true, false },
                { true, true, true, false, false } };
        int[][][] colourGridInt = {
                { { 0, 0, 0, 1, 0 }, { 0, 0, 0, 0, 1 }, { 0, 1, 0, 0, 1 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } },
                { { 0, 0, 0, 1, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 0, 1, 0 }, { 1, 0, 0, 0, 0 }, { 0, 1, 1, 0, 0 } } };

        boolean[][][] colourGrid = new boolean[2][5][5];
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 5; ++j) {
                for (int k = 0; k < 5; ++k) {
                    colourGrid[i][j][k] = colourGridInt[i][j][k] == 1;
                }
            }
        }
        int[][] initShade = { { 0, 0, 0, 1, 0 }, { 0, 1, 0, 1, 0 }, { 0, 1, 0, 1, 0 }, { 0, 0, 1, 0, 0 },
                { 0, 1, 1, 0, 0 } };
        int[][] buttonGrid = { { -1, -1, 0, -1, -1 }, { -1, -1, -1, -1, -1 }, { 1, -1, -1, -1, 1 },
                { -1, -1, -1, -1, -1 }, { -1, -1, 0, -1, -1 } };
        Cell target = new Cell(2, 2);
        Cell[] ballPositions = { new Cell(4, 0), new Cell(0, 4) };
        Board board = new Board(5, 5, 2, 2, initShade, colourGrid, buttonGrid, grid, target);
        State state = new State(board, ballPositions);

        GamePlayer gamePlayer = new GamePlayer(board);
        gamePlayer.playInteractively(state);
    }
}