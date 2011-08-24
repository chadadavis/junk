
import java.io.*;
import java.net.*;

public class iptest {

	public static void main(String[] args) {

	BufferedReader url;
	String line;

	try {
		url = 
		new BufferedReader(new InputStreamReader(
						   ( new URL("http://localhost/test.html")).openStream()));

		line = url.readLine();
		while (line != null) {
			System.out.println(line);
			line = url.readLine();
		}
	} catch (Exception e) {
		System.out.println(e);
	}
	
	}
	
}

		
