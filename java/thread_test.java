
import java.awt.*;
import java.awt.event.*;

public class thread_test implements Runnable {

	public thread_test() {}

	public void run() {
		for (int i = 0; i < 10; i++) {
			synchronized (this) {
				try {
					this.wait();
					System.out.println("a " + this);			
				} catch (Exception e) {
					System.out.println(e);
				} // end of try-catch
			} // syn
		} // end of for ()
	}

    public static void main (String[] args) {
		Runnable r = new thread_test();
		(new Thread(r)).start();
		for (int i = 0; i < 10; i++) {
			synchronized (r) {
				try {
					Thread.sleep(2000);
					System.out.println("b " + r);
					r.notifyAll();
				} catch (Exception e) { 
					System.out.println(e);
				} // end of try-catch
			} // syn
		} // end of for ()
		
	} // end of main ()
	
}


