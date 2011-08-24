
import java.awt.*;
import java.awt.event.*;

public class a extends Frame implements WindowListener {

	public a() {}

	public void windowClosing (WindowEvent e) {
		dispose();
		System.exit(0);
	}
	public void windowClosed (WindowEvent e) { }
	public void windowOpened (WindowEvent e) { }
	public void windowIconified (WindowEvent e) { }
	public void windowDeiconified (WindowEvent e) { }
	public void windowActivated (WindowEvent e) { }
	public void windowDeactivated (WindowEvent e) { }

	public static void main (String[] args) {
		a alpha = new a();

		alpha.setLayout(new FlowLayout());
		Button b = new Button("Press me!");

		alpha.addWindowListener(alpha);

		b.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					System.exit(0);
				}
			});

		alpha.add(b);
		//		a.setSize(400,300);
		alpha.pack();
		alpha.setVisible(true);
		//		f.setSize(new Dimension(400, 400));
		//		f.show();
	} // end of main ()
	
}

