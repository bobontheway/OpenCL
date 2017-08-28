package idcard;
import framework.*;

public class IDCard extends Product {
	private String owner;

	IDCard(String owner) {
		System.out.println("Making " + owner + "'s ID Card");
		this.owner = owner;
	}

	public void use() {
		System.out.println("Using " + owner + "'s ID Card");
	}

	public String getOwner() {
		return owner;
	}
}

