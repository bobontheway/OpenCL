// 只解决指定编号的问题
public class SpecialSupport extends Support {
	private int number;

	// 指定能解决的编号
	public SpecialSupport(String name, int number) {
		super(name);
		this.number = number;
	}

	protected boolean resolve(Trouble trouble) {
		if (trouble.getNumber() == number) {
			return true;
		} else {
			return false;
		}
	}
}
