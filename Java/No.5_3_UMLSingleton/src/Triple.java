// 使用一个包含了撒个元素的数组来保存 Triple 的实例
public class Triple {
	private static Triple[] triple = new Triple[] {
		new Triple(0),
		new Triple(1),
		new Triple(2)
	};

	private int id;
	private Triple(int id) {
		System.out.println("The instance " + id + " is created.");
		this.id = id;
	}

	public static Triple getInstance(int id) {
		return triple[id];
	}

	public String toString() {
		return "[Triple id=" + id + "]";
	}
}

// 问题：
// 1 实例化只能通过构造函数来完成，如何控制实例的个数？
// - 在静态函数中一次性实例化 3 个 Triple 类的对象。

// 1.1 是否有其它的方式来实例化，不依赖构造函数？

// 2 如何将实例的编号和对象关联起来？是否可以使用 HashMap？
// - 建立一个数组，根据获取实例来返回对应的对象。
