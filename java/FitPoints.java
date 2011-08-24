/*
  Draws a regression line through a set of points
*/

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.util.*;

public class FitPoints 
	extends JFrame 
	implements ActionListener, MouseListener {

	public int getx1() { return x1; }
	public int gety1() { return y1; }
	public int getx2() { return x2; }
	public int gety2() { return y2; }
	public LinkedList getPoints() { return points; }

	private void calculate() {
		iter = points.listIterator(0);
		int n = points.size();
		int sx = 0, sy = 0, sxx = 0, sxy = 0;
		Point current;
		while (iter.hasNext()) {
			current = (Point)iter.next();
			int x = current.x;
			int y = current.y;
			sx += x;
			sy += y;
			sxx += x * x;
			sxy += x * y;
		}
		int d = n * sxx - sx * sx;
		int s2 = sy * sxx - sx * sxy;
		int s1 = n * sxy - sx * sy;

		if (d == 0) {
			x1 = y1 = x2 = y2 = 0;
			return;
		} // end of if ()

		x1 = 0; 
		y1 = s2 / d;
		x2 = canvas.getSize().width;
		y2 = y1 + x2 * s1 / d;
	}
	
	public FitPoints(String title) {
		super(title);
		
		points = new LinkedList();

		JPanel main_panel = new JPanel(new BorderLayout());
		JPanel control_panel = new JPanel(new FlowLayout());
		canvas = new MyCanvas(this);

		getContentPane().add(main_panel);
		main_panel.add(control_panel, "North");
		main_panel.add(canvas, "Center");

		undo = new JButton("Undo");
		clear = new JButton("Clear");
		quit = new JButton("Quit");

		undo.addActionListener(this);
		clear.addActionListener(this);
		quit.addActionListener(this);
		canvas.addMouseListener(this);

		control_panel.add(undo);
		control_panel.add(clear);
		control_panel.add(quit);

		addWindowListener(new WindowAdapter() { 
				public void windowClosing(WindowEvent e) {
					System.exit(0);
				}});
		
		pack();
		setVisible(true);

	} /* public void FitPoints() */

	public FitPoints() {
		this("FitPoints");
	} /* public FitPoints() */

	public void actionPerformed(ActionEvent event) {
		Object source = event.getSource();
		if (source == quit) {
			System.exit(0);
		} else if (source == undo ) {
			if (points.size() > 0) {
				points.removeLast();
				calculate();
				canvas.repaint();				   
			} // end of if ()
		} else if (source == clear) {
			System.out.println("Clearing");
			points = new LinkedList();
			x1 = y1 = x2 = y2 = 0;
			canvas.repaint();
		} else {
			// TODO ??
		} // end of else
		
	} /* public void actionPerformed(ActionEvent event) */

	public void mouseClicked (MouseEvent e) {
		points.add(new Point(e.getX(), e.getY()));
		calculate();
		canvas.repaint();				   
	}
	
	public void mouseReleased (MouseEvent e) { }
	public void mousePressed (MouseEvent e) { }
	public void mouseEntered (MouseEvent e) { }
	public void mouseExited (MouseEvent e) { }
	
	public static void main(String[] args)
	{
		FitPoints frame = new FitPoints("FitPoints");
	 
	} /* public static void main(String[] args) */

	private JButton undo;
	private JButton clear;
	private JButton quit;
	private MyCanvas canvas;
	private LinkedList points;
	private ListIterator iter;
	private int x1;
	private int x2;
	private int y1;
	private int y2;
	
} /* class FitPoints  */

class MyCanvas extends Canvas {

	public MyCanvas(FitPoints ref) {
		app = ref;
	}

	public void paint(Graphics g) {

		points = app.getPoints();
		iter = points.listIterator(0);

		setBackground(Color.WHITE);
		g.setColor(Color.BLACK);

		// draw the points
		Point current;
		while (iter.hasNext()) {
			current = (Point)iter.next();
			g.fillOval(current.x, current.y, 5, 5);
		}
		g.setColor(Color.RED);
		g.drawLine (app.getx1(), app.gety1(), app.getx2(), app.gety2());
	}

	public Dimension getMinimumSize() {
		return new Dimension(100, 100);
	}

	public Dimension getPreferredSize() {
		//return getMinimumSize();
		return new Dimension(500, 500);
	}
	private String s = "1";
	private FitPoints app;
	private LinkedList points;
	private ListIterator iter;

}
		
