/**
 *
 *	@author		Chad Davis <cad8@dana.ucc.nau.edu>
 *	
 *
 *	Copyright (C) 1999 by Caffeine-Ware
 *
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *	See http://www.gnu.org/copyleft/gpl.html for more information.
 *
 *
 *******************************************************************************
 *******************************************************************************
 *
 *
 *	This file ... 
 *
 *
 *
 */


/*
 *	The following comment is maintained by the RCS revision system	
 *
 *	$Id: SwingTemplate.java,v 1.1 2002/01/28 05:19:38 cad8 Exp cad8 $
 *
 *	$Source: /home/cad8/doc/devel/java/RCS/SwingTemplate.java,v $
 *
 *	$Log: SwingTemplate.java,v $
 *	Revision 1.1  2002/01/28 05:19:38  cad8
 *	Initial revision
 *
 *
 *
 */


/* Package membership */

//package ...;


/* Java API imports */

import java.awt.Color;
import java.awt.BorderLayout;
import java.awt.event.*;
import javax.swing.*;
import java.util.Locale;
import java.util.ResourceBundle;
import java.text.NumberFormat;


/* Other imports */

//import ...;


/**
 * This class ... 
 *
 * @author Chad Davis <cad8@dana.ucc.nau.edu>
 * @version 1.0
 */
public class SwingTemplate extends JFrame implements ActionListener
{

	
	//////////////////////////////////////////////////////////////////
	//	Instance variables
	//////////////////////////////////////////////////////////////////


	JPanel panel;


	//////////////////////////////////////////////////////////////////
	//	Class variables
	//////////////////////////////////////////////////////////////////


        static String i18nCountry = new String("US");
        static String i18nLanguage = new String("en");
        static Locale i18nCurrentLocale;
        static NumberFormat i18nNumFormat;
        static NumberFormat i18nCurrencyFormat;
        static ResourceBundle i18nResBundle;
        static String i18nBundleName = "Messages";


	//////////////////////////////////////////////////////////////////
	//	Instance Methods
	//////////////////////////////////////////////////////////////////





        //////////////////////////////////////////////////////////////////
        //      Class Methods
        //////////////////////////////////////////////////////////////////


	/**
	 * This Method ...
	 */
	public static void main(String[] args)
	{

                i18nCurrentLocale = new Locale(i18nLanguage, i18nCountry);
                i18nNumFormat = NumberFormat.getNumberInstance(i18nCurrentLocale);
                i18nCurrencyFormat = NumberFormat.getCurrencyInstance(i18nCurrentLocale);
                i18nResBundle = ResourceBundle.getBundle(i18nBundleName, i18nCurrentLocale);

		SwingTemplate frame = new SwingTemplate();
		frame.setTitle("SwingTemplate");
	
		// This allows you to close the window
		frame.addWindowListener(
			new WindowAdapter()
			{
				public void windowClosing(WindowEvent e) 
				{
					System.exit(0);
				}
			}
		);

		// This allows you to see the frame
		frame.pack();
		frame.setVisible(true);

	} /* public static void main(String[] args) */


	/**
	 * This method ...
	 */
	public void actionPerformed(ActionEvent event)
	{

		Object source = event.getSource();

	} /* public void actionPerformed(ActionEvent event) */


        /**
         * This method ...
         */
        protected void finalize() throws Throwable
        {
                super.finalize();

        } /* protected void finalize() throws Throwable */


	//////////////////////////////////////////////////////////////////
	//	Constructors
	//////////////////////////////////////////////////////////////////


	/**
	 * Standard Constructor
	 *
	 */
	public SwingTemplate(String title)
	{

		super(title);
		panel = new JPanel();
		panel.setLayout(new BorderLayout());
		getContentPane().add(panel);

	} /* public void SwingTemplate() */


	/**
	 * Default Constructor
	 *
	 */
	public SwingTemplate()
	{

		this("SwingTemplate");

	} /* public SwingTemplate() */


} /* class SwingTemplate  */


