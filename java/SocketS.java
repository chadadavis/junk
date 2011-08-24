

import java.net.*;
import java.io.*;
import java.util.*;

public class SocketS {

	public static void main(String[] args) {

		try {

			ServerSocket s = new ServerSocket(4000);
			Socket c = s.accept();
			PrintWriter p = new PrintWriter(new BufferedWriter(new OutputStreamWriter(c.getOutputStream())));


			p.println("Hello");			
			p.println("Testing");
			p.println(new Date());
			p.flush();

			p.close();
			c.close();

		} catch (Exception e) {
			System.out.println(e);
		}


		


	}
	
}
