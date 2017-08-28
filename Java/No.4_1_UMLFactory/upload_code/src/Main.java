import framework.*;
import idcard.*;

public class Main {
	public static void main(String[] args) {
		// 返回抽象类
		Factory factory = new IDCardFactory();

		// 抽象类调用子类的实现
		Product card1 = factory.create("XiaoMing");
		Product card2 = factory.create("XiaoHong");
		Product card3 = factory.create("XiaoGong");
		System.out.println();

		// 产品抽象方法的实现
		card1.use();
		card2.use();
		card3.use();
	}
}
