public class Main {
	public static void main(String[] args) {
		// 我们使用了 Print 接口，并且不知道 PrintBanner 是如何实现的，这样就
		// 可以在不用对 Main 类进行修改的情况下改变 PrintBanner 类的具体实现
		Print p = new PrintBanner("Java");
		p.printWeak();
		p.printStrong();
	}
}
