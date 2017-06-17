public class Main {
	public static void main(String[] args) {
		// 我们使用了 Print 接口，并且不知道 PrintBanner 是如何实现的，
		// 这样就可以在不用对 Main 类进行修改的情况下改变 PrintBanner 
		// 类的具体实现

		// 使用 Print 接口而不是 PrintBanner 类表明了程序的意图，只使用
		// Print 接口中的方法，并不使用 PrintBanner 类中的方法
		Print p = new PrintBanner("Java");
		p.printWeak();
		p.printStrong();
	}
}
