public class PrinterProxy implements Printable {
	private String name;
	private Printer real;

	// 构造函数
	public PrinterProxy() {
	}

	public PrinterProxy(String name) {
		this.name = name;
	}

	public synchronized void setPrinterName(String name) {
		if (real != null) {
			real.setPrinterName(name); // 同时设置当事人的名字
		}

		this.name = name; // 设置代理人的名字
	}

	public String getPrinterName() {
		return name;
	}

	public void print(String string) {
		realize(); // 实现
		real.print(string);
	}

	// 生成当事人的实例
	private synchronized void realize() {
		if (real == null) {
			real = new Printer(name);
		}
	}
}
