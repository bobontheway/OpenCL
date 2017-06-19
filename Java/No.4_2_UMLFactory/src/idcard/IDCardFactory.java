package idcard;
import framework.*;
import java.util.*;

public class IDCardFactory extends Factory {
	private List<IDCard> owners = new ArrayList<IDCard>();

	protected Product createProduct(String owner, String id) {
		return new IDCard(owner, id);
	}

	protected void registerProduct(Product product) {
		owners.add((IDCard)product);
	}

	public List getOwners() {
		return owners;
	}
}
