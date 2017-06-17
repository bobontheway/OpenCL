import java.io.*;

public class Main {
	public static void main(String[] args) {
		FileIO f = new FileProperties();

		try {
			f.readFromFile("file.txt");
			f.setValue("year", "2017");
			f.setValue("month", "6");
			f.setValue("day", "17");
			f.writeToFile("newfile.txt");
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
