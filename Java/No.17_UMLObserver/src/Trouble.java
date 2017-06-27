// 表示发生问题的类

public class Trouble {
	private int number;

	public Trouble(int number) {
		this.number = number;
	}

	public int getNumber() {
		return number;
	}

	public String toString() {
		return "[Trouble " + number + "]";
	}
}
