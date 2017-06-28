import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class BigChar {
	private char charname; // 字符名称
	private String fontdata; // 字体数据

	// 构造函数
	public BigChar(char charname) {
		this.charname = charname;

		try {
			BufferedReader reader = new BufferedReader(
				new FileReader("big" + charname + ".txt"));

			// 逐行读取
			String line;
			StringBuffer buf = new StringBuffer();
			while ((line = reader.readLine()) != null) {
				buf.append(line);
				buf.append("\n");
			}

			reader.close();
			this.fontdata = buf.toString();
		} catch (IOException e) {
			this.fontdata = charname + "?";
		}
	}

	// 显示字符，字母之间隔一空行
	public void print() {
		System.out.println(fontdata);
	}
}
