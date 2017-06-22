// 该类用来表示参与选手

public class Player {
	private String name;
	private Strategy strategy;
	private int wincount;
	private int losecount;
	private int gamecount;

	public Player(String name, Strategy strategy) {
		this.name = name;
		this.strategy = strategy;
	}

	public Hand nextHand() {
		return strategy.nextHand();
	}

	public void win() { // 胜
		strategy.study(true);
		wincount++;
		gamecount++;
	}

	public void lose() { // 负
		strategy.study(false);
		losecount++;
		gamecount++;
	}

	public void even() { // 平
		gamecount++;
	}

	public String toString() {
		return "[" + name + ":" + gamecount + " games, " + wincount +
			" win, " + losecount + " lose" + "]";
	}
}
