// 表示一个字符串的类。在实例化对象时，可以传入不同的字符串作为参数

public class BigString {
	// 大型字符的数组
	private BigChar[] bigchars;

	// 构造函数
	public BigString(String string) {
		bigchars = new BigChar[string.length()];
		BigCharFactory factory = BigCharFactory.getInstance();
		for (int i = 0; i < bigchars.length; i++) {
			bigchars[i] = factory.getBigChar(string.charAt(i));
		}
	}

	// 显示
	public void print() {
		for (int i = 0; i < bigchars.length; i++) {
			bigchars[i].print();
		}
	}
}
