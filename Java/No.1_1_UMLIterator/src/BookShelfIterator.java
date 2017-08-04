// 迭代书架上的书（对书架上的书进行操作）。在构造函数中，书架（集合）作为参数传入
public class BookShelfIterator implements Iterator {
	// 书架，以及指向书架上书的当前下标
	private BookShelf bookShelf;
	private int index;

	// 构造对象时初始化私有字段，和 BookShelf 为聚合关系
	public BookShelfIterator(BookShelf bookShelf) {
		this.bookShelf = bookShelf;
		this.index = 0;
	}

	// 判断书架上是否还有下一本书
	public boolean hasNext() {
		if (index < bookShelf.getLength()) {
			return true;
		} else {
			return false;
		}
	}

	// 返回迭代器当前所指向的书，并让迭代器指向下一本书
	public Object next() {
		Book book = bookShelf.getBookAt(index);
		index++;
		return book;
	}
}

