// 该类用来生成票的编号
public class TicketMaker {
	private int ticket = 1000;
	private static TicketMaker singleton = new TicketMaker();

	private TicketMaker() {
	}

	public static TicketMaker getInstance() {
		return singleton;
	}

	// 防止被并行执行
	public synchronized int getNextTicketNumber() {
		return ticket++;
	}
}
