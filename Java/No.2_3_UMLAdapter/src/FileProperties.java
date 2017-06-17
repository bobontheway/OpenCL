import java.io.*;
import java.util.*;

public class FileProperties extends Properties implements FileIO {
	// 从流中取出属性
	public void readFromFile(String filename) throws IOException {
		load(new FileInputStream(filename));
	}

	// 将属性写入流中
	public void writeToFile(String filename) throws IOException {
		store(new FileOutputStream(filename), "written by FileProperties");
	}

	// 为属性 key 设置属性值
	public void setValue(String key, String value) {
		setProperty(key, value);
	}

	// 在属性列表中，查找 key 指定的属性值
	public String getValue(String key) {
		return getProperty(key, "");
	}
}

// 问题：
// 什么是流？如何使用流，如何操作？
//（1）什么是流，可以参考 FileInputStream 和 FileOutputStream 类
