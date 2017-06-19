package idcard;
import framework.*;
import java.util.*;

public class IDCardFactory extends Factory {
	// 使用默认的初始容量构造一个空的 HashMap
	private HashMap<Integer, String> database = new HashMap<Integer, String>();
	private int serial = 100;

	protected Product createProduct(String owner) {
		return new IDCard(owner, serial++);
	}

	protected void registerProduct(Product product) {
		IDCard card = (IDCard)product;
		database.put(new Integer(card.getSerial()), card.getOwner());
	}

	public HashMap getDatabase() {
		return database;
	}
}
