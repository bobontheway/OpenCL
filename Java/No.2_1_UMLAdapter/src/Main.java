public class Main {
	public static void main(String[] args) {
		// 针对接口编程，而不是具体实现
		Print p = new PrintBanner("Java");
		p.printWeak();
		p.printStrong();
	}
}
