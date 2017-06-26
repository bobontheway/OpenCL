// 解决编号小于 limit 的问题
public class LimitSupport extends Support {
	private int limit;

	// 编号小于 limit 的问题才会解决
	public LimitSupport(String name, int limit) {
		super(name);
		this.limit = limit;
	}

	// 解决问题的方法
	protected boolean resolve(Trouble trouble) {
		if (trouble.getNumber() < limit) {
			return true;
		} else {
			return false;
		}
	}
}
