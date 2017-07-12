public class RoadBike extends Bicycle {
	// In millimeters(mm，毫米)
	private int tireWidth; // 轮子宽度

	public RoadBike(int startCadence, int startSpeed,
		int startGear, int newTireWidth) {
		super(startCadence, startSpeed, startGear);
		this.setTireWidth(newTireWidth);
	}

	public int getTireWidth() {
		return this.tireWidth;
	}

	public void setTireWidth(int newTireWidth) {
		this.tireWidth = newTireWidth;
	}

	public void printDescription() {
		super.printDescription();
		System.out.println("The RoadBike" + "has " +
			getTireWidth() + "  MM tires.");
	}
}

