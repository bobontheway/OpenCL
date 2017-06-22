import java.util.Random;

// 如果上一局的手势获胜了，则下一局的手势与上局相同。
// 如果上一局的手势输了，则下一局就随机出手势
public class WinningStrategy implements Strategy {
	private Random random; // 随机数生成器
	private boolean won = false; // 保存了上一局猜拳的输赢结果
	private Hand prevHand; // 保存的是上一局出的手势

	public WinningStrategy(int seed) {
		// 创建一个新的随机数生成器
		random = new Random(seed);
	}

	// 获取下一局要出的手势
	public Hand nextHand() {
		if (!won) {
			// 返回伪随机数，范围从 0（包含） 到 3（不包含）
			prevHand = Hand.getHand(random.nextInt(3));
		}
		return prevHand; // 如果获胜，则下一局的手势与上局相同
	}

	// 学习 “上一局的手势是否获胜了”
	public void study(boolean win) {
		won = win;
	}


}
