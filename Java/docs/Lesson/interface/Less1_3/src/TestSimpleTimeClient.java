public class TestSimpleTimeClient {
	public static void main(String[] args) {
		TimeClient myTimeClient = new SimpleTimeClient();
		System.out.println("Current time: " + myTimeClient);
		System.out.println("Time in California: " +
			myTimeClient.getZonedDateTime("Blah blah").toString());
	}
}
