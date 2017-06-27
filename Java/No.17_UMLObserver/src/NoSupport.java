// 这是一个永远不解决问题的类
public class NoSupport extends Support {
	public NoSupport(String name) {
		super(name);
	}

	// 自己什么也不处理，永远不解决问题
	protected boolean resolve(Trouble trouble) {
		return false;
	}
}
