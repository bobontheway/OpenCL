import java.util.Iterator;
import java.util.ArrayList;

public class Directory extends Entry {
	private String name;
	private ArrayList<Entry> directory = new ArrayList<Entry>();

	public Directory(String name) {
		this.name = name;
	}

	public String getName() {
		return name;
	}

	public int getSize() {
		int size = 0;

		// 返回迭代器，用来以正确的顺序遍历列表中的元素
		Iterator it = directory.iterator();
		while (it.hasNext()) { // 如果迭代器中还有元素，返回 true
			Entry entry = (Entry)it.next(); // 返回迭代器中下一个元素
			size += entry.getSize();
		}
		return size;
	}

	public Entry add(Entry entry) {
		directory.add(entry);
		return this;
	}

	protected void printList(String prefix) {
		System.out.println(prefix + "/" + this);
		Iterator it = directory.iterator();
		while (it.hasNext()) {
			Entry entry = (Entry)it.next();
			entry.printList(prefix + "/" + name);
		}
	}
}
