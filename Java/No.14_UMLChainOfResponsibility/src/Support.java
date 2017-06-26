// 用来解决问题的抽象类，是职责链上的对象
public abstract class Support {
	private String name; // 解决问题的实例的名称
	private Support next; // 要推卸给的目标对象

	public Support(String name) {
		this.name = name;
	}

	// 设置要推卸给的目标对象
	public Support setNext(Support next) {
		this.next = next;
		return next;
	}

	// 解决问题的步骤（Template Mode）
	public final void support(Trouble trouble) {
		if (resolve(trouble)) { // 问题已经被处理
			done(trouble);
		} else if (next != null) {
			next.support(trouble); // 将问题转交给下一个对象
		} else {
			fail(trouble);
		}
	}

	// 显示字符串
	public String toString() {
		return "[" + name + "]";
	}

	// 解决问题的方法，需要子类去实现
	protected abstract boolean resolve(Trouble trouble);
	
	// 完成
	protected void done(Trouble trouble) {
		System.out.println(trouble + " is resolved by " + this + ".");
	}

	// 未完成
	protected void fail(Trouble trouble) {
		System.out.println(trouble + "cannot be resolved.");
	}
}
