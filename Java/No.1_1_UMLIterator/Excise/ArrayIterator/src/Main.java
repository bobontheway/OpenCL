public class Main {
	public static void main(String[] args) {
		int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

		ArrayIterator it = new ArrayIterator(array);
		while (it.hasNext()) {
			System.out.print(it.next() + "_");
		}

		//for (int i = 0; i < array.length; i++) {
		//	System.out.print(array[i]);
		//	//System.out.print(array[i] + "  ");
		//}
		System.out.println();
	}
}
