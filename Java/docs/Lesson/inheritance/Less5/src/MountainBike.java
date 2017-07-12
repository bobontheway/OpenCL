public class MountainBike extends Bicycle {
	// the MountainBike subclass adds one field
	private String suspension; // 减震器

	public MountainBike(int startCadence, int startSpeed,
		int startGear, String suspensionType) {
		super(startCadence, startSpeed, startGear);
		this.setSuspension(suspensionType);
	}

	public String getSuspension() {
		return this.suspension;
	}

	public void setSuspension(String suspensionType) {
		this.suspension = suspensionType;
	}

	public void printDescription() {
		super.printDescription();
		System.out.println("The " + "MountainBike has a " +
			getSuspension() + " suspension.");
	}
}
