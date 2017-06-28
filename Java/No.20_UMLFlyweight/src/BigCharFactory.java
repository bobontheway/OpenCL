import java.util.HashMap;

// 用来管理 BigChar 的类
public class BigCharFactory {
	// 管理已经生成的 BigChar 的实例
	private HashMap<String, BigChar> pool = new HashMap<String, BigChar>();

	// singleton 模式
	private static BigCharFactory singleton = new BigCharFactory();

	// 构造函数。单例模式，防止其它类对它进行实例化操作
	private BigCharFactory() {
	}

	// 获取唯一的实例
	public static BigCharFactory getInstance() {
		return singleton;
	}

	// 生成（共享）BigChar 类的实例
	public synchronized BigChar getBigChar(char charname) {
		BigChar bc = (BigChar)pool.get("" + charname);
		if (bc == null) {
			bc = new BigChar(charname); // 生成 BigChar 的实例
			pool.put("" + charname, bc);
		}
		return bc;
	}
}
