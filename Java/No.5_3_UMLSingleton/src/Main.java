public class Main {
	public static void main(String[] args) {
		System.out.println("Start.");

		for (int i = 0; i < 10; i++) {
			Triple triple =  Triple.getInstance(i % 3);
			System.out.println(i + ": " + triple);
		}

		System.out.println("End.");
	}
}

// 问题：
// 在调用 System.out.println 时，传递的参数为 triple 对象，怎么调用了 toString
// 函数？
