// 该类表示书架。需要将该类作为集合进行处理，故实现了 Aggregate 接口
public class BookShelf implements Aggregate {
	private Book[] books;
	private int last = 0;

	// books 数组的大小在生成 BookShelf 实例时被指定
	public BookShelf(int maxsize) {
		this.books = new Book[maxsize];
	}

	public Book getBookAt(int index) {
		return books[index];
	}

	public void appendBook(Book book) {
		this.books[last] = book;
		last++;
	}

	public int getLength() {
		return last;
	}

	// 生成并返回 BookShelfIterator 的实例作为 BookShelf 类对应的 Iterator
	public Iterator iterator() {
		return new BookShelfIterator(this);
	}
}
