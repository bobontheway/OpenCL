public class ArrayIterator {
	private int[] mArray;
	private int index;

	public ArrayIterator(int[] array) {
		this.mArray = array;
		this.index = 0;
	}

	public boolean hasNext() {
		if (index < mArray.length)
			return true;
		else
			return false;
	}

	public int next() {
		int val = mArray[index];
		index++;
		return val;
	}
}
