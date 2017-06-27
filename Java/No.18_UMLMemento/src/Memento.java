import java.util.*;
// 获取当前所持有的水果

// 表示 Gamer 状态的类
public class Memento {
	int money; // 所持金钱
	ArrayList<String> fruits; // 获得的水果

	// 获取玩家当前所持有的金钱数目
	public int getMoney() {
		return money;
	}

	Memento(int money) {
		this.money = money;
		this.fruits = new ArrayList<String>();
	}

	// 添加所获得的水果
	void addFruit(String fruit) {
		fruits.add(fruit);
	}

	// 获取当前所持有的水果
	List<String> getFruits() {
		return (List)fruits.clone(); // 创建并返回该对象的拷贝
	}
}
