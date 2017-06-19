import framework.*;
import idcard.*;

public class Main {
	public static void main(String[] args) {
		// 返回抽象类
		Factory factory = new IDCardFactory();
		// 抽象类调用子类的实现
		Product card1 = factory.create("小明", "20062371");
		Product card2 = factory.create("小红", "20061772");
		Product card3 = factory.create("小刚", "20068765");
		System.out.println();

		// 产品抽象方法的实现
		card1.use();
		card2.use();
		card3.use();
	}
}
