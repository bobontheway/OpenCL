// 生成一个 Gamer 类的实例并进行游戏，重复调用 Gamer 的
// bet 方法，并显示 Gamer 所持有的金钱
public class Main {
	public static void main(String[] args) {
		// 最初所持有的金钱数为 100
		Gamer gamer = new Gamer(100);
		// 保存最初的状态
		Memento memento = gamer.createMemento();

		for (int i = 0; i < 100; i++) {
			System.out.println("==== " + i); // 显示投骰子的次数
			System.out.println("当前状态：" + gamer); // 玩家当前状态(toString)

			gamer.bet(); // 进行游戏（头骰子）

			System.out.println("所持有的金钱为" + gamer.getMoney() + "元。");

			// 决定如何处理 Memento
			if (gamer.getMoney() > memento.getMoney()) {
				System.out.println("（所持有的金钱数增加了，保持当前游戏状态）");
				memento = gamer.createMemento();
			} else if (gamer.getMoney() < memento.getMoney() / 2) {
				System.out.println("（所持有的金钱少了许多，将游戏恢复至以前状态）");
				gamer.restoreMemento(memento);
			}

			// 当代一段时间
			try {
				Thread.sleep(1000); // 以毫秒为单位
			} catch (InterruptedException e) {
			}

			System.out.println("");
		}
	}
}

