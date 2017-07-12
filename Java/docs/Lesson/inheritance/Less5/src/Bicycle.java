public class Bicycle {
	// the Bicycle class has three fields
	public int cadence; // 韵律，节奏
	public int gear; // 齿轮
	public int speed; // 速度

	public Bicycle(int cadence, int speed, int gear) {
		this.cadence = cadence;
		this.speed = speed;
		this.gear = gear;
	}

	public void printDescription() {
		System.out.println("\nBike is " + "in gear " + this.gear +
			" with a cadence of " + this.cadence +
			" and travelling at a speed of " + this.speed + ". ");
	}
}
