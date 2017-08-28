public class Singleton {
	// 初始化行为仅在该类被加载时进行一次
	private static Singleton singleton = new Singleton();

	// 禁止从 Singleton 外部调用构造函数
	private Singleton() {
		System.out.println("Creating an instance.");
	}

	// 如果是 static 方法，可以直接使用类名字来调用
	public static Singleton getInstance() {
		return singleton;
	}
}
