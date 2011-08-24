
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class b  {

// 	public void windowClosing (WindowEvent e) {
// 		dispose();
// 		System.exit(0);
// 	}
// 	public void windowClosed (WindowEvent e) { }
// 	public void windowOpened (WindowEvent e) { }
// 	public void windowIconified (WindowEvent e) { }
// 	public void windowDeiconified (WindowEvent e) { }
// 	public void windowActivated (WindowEvent e) { }
// 	public void windowDeactivated (WindowEvent e) { }

	public static void main (String[] args) {
		JFrame f = new JFrame("stuff");
		Container content = f.getContentPane();
		content.setLayout(new FlowLayout());

		JButton b = new JButton("Press me!");

		b.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					System.exit(0);
				}
			});

		content.add(b);

    f.setDefaultCloseOperation (JFrame.DISPOSE_ON_CLOSE);
    f.setSize(400,150);
    f.setVisible(true);

	} // end of main ()
	
}

