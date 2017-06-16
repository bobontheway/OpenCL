import java.util.ArrayList;

// 该类表示书架。需要将该类作为集合进行处理，故实现了 Aggregate 接口
public class BookShelf implements Aggregate {
	// 聚合类，泛型
	private ArrayList<Book> books;

	// 构造一个具有指定初始大小的空列表
	public BookShelf(int initialSize) {
		this.books = new ArrayList<Book>(initialSize);
	}

	// 返回列表中指定位置的元素
	public Book getBookAt(int index) {
		return (Book)books.get(index);
	}

	// 将指定的元素添加到列表的末端
	public void appendBook(Book book) {
		books.add(book);
	}

	// 返回列表中的元素个数
	public int getLength() {
		return books.size();
	}

	// 生成并返回 BookShelfIterator 的实例作为 BookShelf 类对应的 Iterator
	public Iterator iterator() {
		return new BookShelfIterator(this);
	}
}
