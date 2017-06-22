import java.util.Random;

// 随机出手势，但是每种手势出现的概率会根据以前的猜拳结果而改变

public class ProbStrategy implements Strategy {

	// history

	private Random random;
	private int prevHandValue = 0; // 保留上一局出的手势
	private int currentHandValue = 0; // 当前局出的手势
	// 用于根据过去的胜负来进行概率计算。值越大，表示过去的胜率越高
	// history[上一局出的手势][下一局出的手势] = 两局分别出 X、Y 时胜了的次数
	private int[][] history = {
		{1, 1, 1,},
		{1, 1, 1,},
		{1, 1, 1,},
	};

	public ProbStrategy(int seed) {
		// 创建一个新的随机数生成器
		random = new Random(seed);
	}

	// 获取下一局要出的手势
	public Hand nextHand() {
		int bet = random.nextInt(getSum(currentHandValue));
		int handvalue = 0;

		if (bet < history[currentHandValue][0]) {
			handvalue = 0;
		} else if (bet < history[currentHandValue][1]) {
			handvalue = 1;
		} else {
			handvalue = 2;
		}

		prevHandValue = currentHandValue;
		currentHandValue = handvalue;
		return Hand.getHand(handvalue);
	}

	private int getSum(int hv) {
		int sum = 0;

		for (int i = 0; i < 3; i++) {
			sum += history[hv][i];
		}
		return sum;
	}

	// 学习，上一局手势是否或胜了
	public void study(boolean win) {
		if (win) {
			history[prevHandValue][currentHandValue]++;
		} else {
			// 不是只有一个可能赢吗？
			history[prevHandValue][(currentHandValue + 1) % 3]++;
			history[prevHandValue][(currentHandValue + 2) % 3]++;
		}
	}
}
