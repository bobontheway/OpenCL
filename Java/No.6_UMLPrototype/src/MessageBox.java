import framework.*;

public class MessageBox implements Product {
	private char decochar;

	public MessageBox(char decochar) {
		this.decochar = decochar;
	}

	public void use(String s) {
		int length = s.getBytes().length;

		for (int i = 0; i < length + 4; i++) {
			System.out.print(decochar);
		}
		System.out.println("");
			
		System.out.println(decochar + " " + s + " " + decochar);
		
		for (int i = 0; i < length + 4; i++) {
			System.out.print(decochar);
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
