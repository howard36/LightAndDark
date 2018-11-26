public class Main {
    public static void main(String[] args) {
        Board board = new Board(5, 5, 2, 2);
        GamePlayer gamePlayer = new GamePlayer(board);
        gamePlayer.playInteractivelyHardest();
    }
}