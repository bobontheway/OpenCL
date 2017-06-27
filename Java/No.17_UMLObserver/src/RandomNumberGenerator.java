import java.util.Random;

// 如果不是抽象类，必须实现所继承的抽象类的抽象方法
public class RandomNumberGenerator extends NumberGenerator {
	private Random random = new Random(); // 随机数生成器
	private int number;

	// 获取数值
	public int getNumber() {
		return number;
	}

	// 生成数值
	public void execute() {
		for (int i = 0; i < 20; i++) {
			number = random.nextInt(50);
			notifyObservers();
		}
	}

}
