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

	public static void main(String[] args) {
		Relatable r1 = new RectanglePlus(100, 20);
		Relatable r2 = new RectanglePlus(20, 100);
		if (r1.isLargerThan(r2) == 1) {
			System.out.println("r1 > r2");
		} else if (r1.isLargerThan(r2) == 0) {
			System.out.println("r1 = r2");
		} else {
			System.out.println("r1 < r2");
		}
	}
}
