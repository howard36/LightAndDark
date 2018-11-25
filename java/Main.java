public class Main {
    public static void main(String[] args) {
        System.out.println("In Main\n");
        int[][] initShade = { { 0, 1, 1 }, { 0, 0, 0 }, { 1, 0, 0 } };
        boolean[][][] colourGrid = { { { true, false, false }, { false, false, true }, { false, true, false } },
                { { false, false, false }, { false, true, false }, { true, true, false } } };
        int[][] buttonGrid = { { -1, -1, -1 }, { 0, 1, -1 }, { -1, 0, -1 } };
        Cell target = new Cell(0, 0);
        Cell[] ballPositions = { new Cell(1, 2), new Cell(0, 2) };
        Board board = new Board(3, 3, 2, 2, initShade, colourGrid, buttonGrid, target);
        State state = new State(board, ballPositions);

        GamePlayer gamePlayer = new GamePlayer(board);
        gamePlayer.playInteractively(state);
    }
}