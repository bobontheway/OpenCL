import framework.*;

public class UnderlinePen implements Product {
	private char ulchar;

	public UnderlinePen(char ulchar) {
		this.ulchar = ulchar;
	}

	public void use(String s) {
		int length = s.getBytes().length;

		System.out.println("\"" + s + "\"");

		System.out.print(" ");
		for (int i = 0; i < length; i++) {
			System.out.print(ulchar);
		}
		System.out.println("");
	}

	public Product createClone() {
		Product p = null;

		// 创建并返回对象的副本。“拷贝“ 的具体含义取决与对象的类
		try {
			p = (Product)clone();
		} catch (CloneNotSupportedException e) {
			e.printStackTrace();
		}

		return p;
	}


}
