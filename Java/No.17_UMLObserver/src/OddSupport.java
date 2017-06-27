// 解决奇数编号的类
public class OddSupport extends Support {
	public OddSupport(String name) {
		super(name);
	}

	// 处理奇数编号的问题
	protected boolean resolve(Trouble trouble) {
		if ((trouble.getNumber() % 2) == 1) {
			return true;
		} else {
			return false;
		}
	}
}
