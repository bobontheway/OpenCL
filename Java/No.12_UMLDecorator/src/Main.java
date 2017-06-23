public class Main {
	public static void main(String[] args) {
		Display d1 = new StringDisplay("Hello world.");
		d1.show();
		System.out.println();

		Display d2 = new SideBorder(d1, '#');
		d2.show();
		System.out.println();

		Display d3 = new FullBorder(d2);
		d3.show();
		System.out.println();

		Display d4 =
			new SideBorder(
				new FullBorder(
					new FullBorder(
						new SideBorder(
							new FullBorder(
								new StringDisplay("Hello, World!")
								),
							'*')
						)
					),
				'/');
		d4.show();

	}
}
