import java.util.*;

// 表示玩家的类
public class Gamer {
	private int money; // 所持有的金钱
	private List<String> fruits = new ArrayList<String>(); // 获得的水果
	private Random random = new Random(); // 随机数生成器
	private static String[] fruitsname = {
		"苹果", "葡萄", "香蕉", "橘子",
	};
	
	// 玩家初始的金钱数目
	public Gamer(int money) {
		this.money = money;
	}

	// 获取当前所持金钱
	public int getMoney() {
		return money;
	}

	public void bet() {
		int dice = random.nextInt(6) + 1; // 投骰子
		if (dice == 1) {
			money += 100; // 当骰子点数为 1，金钱会增加
			System.out.println("所持有金钱增加了。");
		} else if (dice == 2) {
			money /= 2; // 当骰子数为 2 时，所持有金钱减半
			System.out.println("所持有金钱减半了。");
		} else if (dice == 6) { // 骰子数为 6 时，获得了水果
			String f = getFruit();
			System.out.println("获得了水果（" + f + "）。");
			fruits.add(f);
		} else { // 骰子结果为 3、4、5 时，则什么都不会发生
			System.out.println("什么都没有发生。");
		}
	}

	// 拍摄快照
	public Memento createMemento() {
		Memento m = new Memento(money);
		Iterator it = fruits.iterator();
		while (it.hasNext()) {
			String f = (String)it.next();
			if (f.startsWith("好吃的")) { // 只保存好吃的水果
				m.addFruit(f);
			}
		}
		return m;
	}

	public void restoreMemento(Memento memento) {
		this.money = memento.getMoney();
		this.fruits = memento.getFruits();
	}

	// 用字符串表示玩家状态，fruits 会输出什么？
	public String toString() {
		return "[money =  " + money + ", fruits = " + fruits + "]";
	}

	// 随机获得一个水果
	private String getFruit() {
		String prefix = "";
		if (random.nextBoolean()) {
			prefix = "好吃的";
		}
		return prefix + fruitsname[random.nextInt(fruitsname.length)];
	}
}
