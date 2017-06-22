// 表示猜拳手势。在该类中，用 int 表示所出的手势：
// 其中 0 表示石头，1 表示剪刀，2 表示布
public class Hand {
	public static final int HANDVALUE_GUU = 0;
	public static final int HANDVALUE_CHO = 1;
	public static final int HANDVALUE_PAA = 2;

	public static final Hand[] hand = {
		new Hand(HANDVALUE_GUU),
		new Hand(HANDVALUE_CHO),
		new Hand(HANDVALUE_PAA),
	};

	private static final String[] name = {
		"石头", "剪刀", "步",
	};

	private int handvalue;
	private Hand(int handvalue) {
		this.handvalue = handvalue;
	}

	public static Hand getHand(int handvalue) {
		return hand[handvalue];
	}

	public boolean isStrongerThan(Hand h) {
		return fight(h) == 1;
	}

	public boolean isWeakerThan(Hand h) {
		return fight(h) == -1;
	}

	// 结果：平-0，胜-1，负-2
	private int fight(Hand h) {
		if (this == h) {
			return 0;
		} else if ((this.handvalue + 1) % 3 == h.handvalue) {
			return 1;
		} else {
			return -1;
		}
	}

	public String toString() {
		return name[handvalue];
	}
}
