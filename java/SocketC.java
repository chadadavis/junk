
import java.net.*;
import java.io.*;

public class SocketC {

	public static void main(String[] args) {

		Socket c;
		BufferedReader r;
		try {
			c = new Socket("localhost", 4000);
			
			r = new BufferedReader(new InputStreamReader(c.getInputStream()));

			String s = r.readLine();
			while (s != null) {
				System.out.println(s);
				s = r.readLine();
			}


			r.close();
			c.close();

		} catch (Exception e) {
			System.out.println(e);
		}



	}
	
}
