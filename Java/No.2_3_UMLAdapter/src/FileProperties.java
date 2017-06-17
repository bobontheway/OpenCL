import java.io.*;
import java.util.*;

public class FileProperties extends Properties implements FileIO {
	public void readFromFile(String filename) throws IOException {
		load(new FileInputStream(filename));
	}

	public void writeToFile(String filename) throws IOException {
		store(new FileOutputStream(filename), "written by FileProperties");
	}

	public void setValue(String key, String value) {
		setProperty(key, value);
	}

	public String getValue(String key) {
		return getProperty(key, "");
	}
}

// 问题：
// 什么是流？如何使用流，如何操作？
//（1）什么是流，可以参考 FileInputStream 和 FileOutputStream 类
