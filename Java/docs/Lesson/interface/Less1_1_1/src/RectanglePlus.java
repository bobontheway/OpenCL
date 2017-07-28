public class RectanglePlus implements Relatable {
	public int width = 0;
	public int height = 0;

	public RectanglePlus(int w, int h) {
		width = w;
		height = h;
	}

	// a method for computing the area of the rectangle
	public int getArea() {
		return width * height;
	}

	// a method required to implement the Relatable interface
	public int isLargerThan(Relatable other) {
		RectanglePlus otherRect = (RectanglePlus)other;
		if (this.getArea() < otherRect.getArea())
			return -1;
		else if (this.getArea() > otherRect.getArea())
			return 1;
		else
			return 0;
	}

	public static Object findLargest(Object object1, Object object2) {
		Relatable obj1 = (Relatable)object1;
		Relatable obj2 = (Relatable)object2;
		if ((obj1).isLargerThan(obj2) > 0)
			return object1;
		else
			return object2;
	}

	public static Object findSmallest(Object object1, Object object2) {
		Relatable obj1 = (Relatable)object1;
		Relatable obj2 = (Relatable)object2;
		if ((obj1).isLargerThan(obj2) < 0)
			return object1;
		else
			return object2;
	}

	public static boolean isEqual(Object object1, Object object2) {
		Relatable obj1 = (Relatable)object1;
		Relatable obj2 = (Relatable)object2;
		if ((obj1).isLargerThan(obj2) == 0)
			return true;
		else
			return false;
	}

	public String toString() {
		return "" + this.getArea();
	}

	public static void main(String[] args) {
		// test equal
		//Relatable r1 = new RectanglePlus(100, 20);
		//Relatable r2 = new RectanglePlus(20, 100);

		// test smallest
		Relatable r1 = new RectanglePlus(100, 200);
		Relatable r2 = new RectanglePlus(200, 200);

		System.out.println("Smallest: " + findSmallest(r1, r2).toString());
		System.out.println("Largest: " + findLargest(r1, r2).toString());

		if (isEqual(r1, r2)) {
			System.out.println("r1 = r2");
		} else {
			System.out.println("r1 != r2");
		}

		/*
		   if (isEqual(r1, r2)) {
		   System.out.println("r1 = r2");
		   }

		   if (r1.isLargerThan(r2) == 1) {
		   System.out.println("r1 > r2");
		   } else if (r1.isLargerThan(r2) == 0) {
		   System.out.println("r1 = r2");
		   } else {
		   System.out.println("r1 < r2");
		   }*/
	}
}
