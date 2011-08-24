#!/usr/bin/env python
#
# Copyright (C) 2004 Mark H. Lyon <mark@marklyon.org>
#
# This file is the Mbox & Maildir to Gmail Loader (GML).
#
# Mbox & Maildir to Gmail Loader (GML) is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# GML is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with GML; if not, write to the Free Software Foundation, Inc.,
# 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
#
# This entire header must remain intact in any distributed copy.

# Origional development thread at Ars Technica:
# http://episteme.arstechnica.com/eve/ubb.x?a=tpc&s=50009562&f=6330927813&m=108000474631
#
# Version 0.1 - 15 Jun 04 16:28 Supports Mbox
# Version 0.2 - 15 Jun 04 18:48 Implementing Magus` suggestion for Maildir
# Version 0.3 - 16 Jun 04 16:17 Implement Rold Gold suggestion for counters
# Version 0.4 - 17 Jun 04 13:15 Add support for changing SMTP server at command line
# Version 0.5 - 25 Jun 04 16:00 Implemented GUI and added support for "Sent" mail
#                               submitted to me by Steven Scott <progoth@gmail.com>
#                               Special Thanks to: Buddy Fortner, Brian Hodgin,
#                               Jamie Zawinski, The Youngblood Brass Band, Diet Pepsi,
#                               and many others for helping in their own way.
#
#                               Some of the GUI widgets come from Pmw, which can be
#                               downloaded at http://pmw.sourceforge.net/
# Version .5b - 30 Jun 04 13:00 Update to solve the missing os import bug reported
#                               by users trying maildir.


import os, sys, mailbox, smtplib, time, Pmw, webbrowser, tkFileDialog, re
from Tkinter import *
#from webbrowser import open_new
#from Pmw import ComboBox, ScrolledText, logicalfont
#from tkFileDialog import askopenfilename


class GmlWin:

   def __init__(self, parent=None):

      self.version = '0.5 - 25 Jun 04 16:00'

      # Let's get everything initilized
      self.root = Tk()
      self.boxtype = ''            # Stores the current box type
      self.rcvorsn = ''  	   # Stores the current recieve or send status
      self.mb      = ''            # The opened mailbox
      self.msg     = ''            # The Message
      self.document= ''            # The Message Body
      self.needath = IntVar()      # 0 no authentication,  1 use authentication
      self.mailbox = ''            # Path to message store
      self.recipnt = ''            # Gmail account to forward to
      self.smtpsvr = ''            # SMTP Server to use
      self.smtpusr = ''            # SMTP Username, required if needath = 1
      self.smtppwd = ''            # SMTP Password, required if needath = 1
      self.running = 0		   # Flag for great justice (0 clear, 1 running, 2 error)
      self.count   = [0,0,0]       # Counters

      self.boxtyps = ('mBox (Netscape, Mozilla, Thunderbird)', 'mBox (Less Strict - Solves Some Problems)',
                      'MailDir (Qmail, others)', 'MMDF (Mutt, SCO Unix)', 'MH (NMH Message System)',
                      'Babyl (Emacs RMAIL)')
      self.rcvtyps = ('Mail I Recieved (Goes to Inbox)', 'Mail I Sent (Goes to Sent Mail)')

      self.main_screen()
      self.root.title('Google Mail Loader')
      mainloop()

   def main_screen(self):

      # Setup Step Labels
      Label(self.root, text="Setup SMTP Server Settings", font=("Helvetica", 10, 'bold'), relief=GROOVE) \
         .grid(	row=0, 	column=0, columnspan=5, sticky=W+E, padx=5, pady=5)
      Label(self.root, text="Configure Your Email File",  font=("Helvetica", 10, 'bold'), relief=GROOVE) \
         .grid(	row=8, 	column=0, columnspan=5, sticky=W+E, padx=5, pady=5)
      Label(self.root, text="Enter Your GMail Address",   font=("Helvetica", 10, 'bold'), relief=GROOVE) \
         .grid(	row=13, column=0, columnspan=5, sticky=W+E, padx=5, pady=5)

      # SMTP Settings
      Label(self.root, text="SMTP Server:").grid(row=1, column=0, sticky=E)
      self.smtpsvr = Entry(self.root); self.smtpsvr.insert(0, 'gsmtp57.google.com');
      self.smtpsvr.grid(row=1, column=1, columnspan=3, sticky=W+E)
      Checkbutton(self.root, text="Requires Authentication", variable=self.needath, command=self.NeedAuth) \
         .grid(row=2, column=1, columnspan=3, sticky=W)
      Label(self.root, text="Username:").grid(row=3, column=0, sticky=E)
      Label(self.root, text="Password:").grid(row=4, column=0, sticky=E)
      self.smtpusr = Entry(self.root, state=DISABLED);
      self.smtpusr.grid(row=3, column=1, columnspan=3, sticky=W+E)
      self.smtppwd = Entry(self.root, state=DISABLED);
      self.smtppwd.grid(row=4, column=1, columnspan=3, sticky=W+E)
      Label(self.root, text=" ").grid(row=5, column=0, sticky=E)

      # Message Store Settings
      self.mailbox = Entry(self.root); self.mailbox.grid(row=9, column=0, columnspan=3, sticky=W+E)
      Button(self.root, text="Find", command=self.FindFile).grid(row=9, column=3, sticky=W+E)
      self.boxtype = Pmw.ComboBox(self.root, label_text = 'File Type:', labelpos = 'nw',
                                  scrolledlist_items = self.boxtyps)
      self.boxtype.selectitem(self.boxtyps[0])
      self.boxtype.grid(row=10, column=0, columnspan=4, sticky=W+E)
      self.rcvorsn = Pmw.ComboBox(self.root, label_text = 'Message Type:', labelpos = 'nw',
                                  scrolledlist_items = self.rcvtyps)
      self.rcvorsn.selectitem(self.rcvtyps[0])
      self.rcvorsn.grid(row=11, column=0, columnspan=4, sticky=W+E)
      Label(self.root, text=" ").grid(row=12, column=0, sticky=E)

      # GMail Account Information
      self.recipnt = Entry(self.root); self.recipnt.insert(0, '@gmail.com');
      self.recipnt.grid(row=14, column=0, columnspan=4, sticky=W+E)
      Label(self.root, text=" ").grid(row=15, column=0, sticky=E)

      # Execute and About
      Button(self.root, text="Send To GMail", command=self.execute) \
         .grid(row=16, column=0, columnspan=2, sticky=W+E)
      Button(self.root, text="Save Log", command=self.saveText) \
         .grid(row=16, column=2, sticky=E)
      Button(self.root, text="About", command=self.openAbout) \
         .grid(row=16, column=3, sticky=E)

      # Output Window
      self.st = Pmw.ScrolledText(self.root, borderframe = 1,
                                 usehullsize = 1, hull_width = 375,
                                 hull_height = 375, text_padx = 10,
                                 text_pady = 10, text_wrap='word',
      		                 hscrollmode='dynamic', vscrollmode='static',
      		                 text_font = Pmw.logicalfont(name='Fixed', size=12))
      self.st.grid(row=0, column=6, sticky=NW, rowspan=17, padx=10, pady=5)

      self.writeHeader()
      self.writeInstructions()


    # Tests to see if the SMTP Authentication option's status,
    # and enables/disables the text boxes accordingly
   def NeedAuth(self):
      if self.needath.get():
         self.smtpusr.config( state = NORMAL )
         self.smtppwd.config( state = NORMAL )
      else:
         self.smtpusr.config( state = DISABLED )
         self.smtppwd.config( state = DISABLED )

    # Test for a correct file using the 'Find' button
    # and populates the text box
   def FindFile(self):
      FileName = tkFileDialog.askopenfilename()
      if (FileName==None or FileName==""):
         return
      self.mailbox.delete(0, END)
      self.mailbox.insert(0, FileName)

    # Opens a new browser window directed at the gml page.
   def openAbout(self):
      webbrowser.open_new('http://www.marklyon.org/gmail')

    # The header and credits
   def writeHeader(self):
      self.st.clear()
      self.Disp("Gmail Loader (GML)")
      self.Disp("By Mark Lyon <mark@marklyon.org>")
      self.Disp("Version: " , self.version)
      self.Disp()

    # Some simple usage information
   def writeInstructions(self):
      self.Disp("To use, complete the options to the left of this message:")
      self.Disp("1) Most users can connect to Google's SMTP server, but you may " \
                "specify your own SMTP server if you desire.")
      self.Disp("2) Locate your email file.  If you are using a MailDir, locate " \
                "one file inside your MailDir.")
      self.Disp("3) Choose the correct file type.  There are two versions of " \
                "mBox.  Use the 'Less Strict' if you have problems with the other.")
      self.Disp("4) Select where you would like your messages to go in Gmail, " \
                "either 'Inbox' or 'Sent Mail'.")
      self.Disp("5) Enter your complete GMail address, and click 'Send To GMail'.")


    # Write to the window or console
   def Disp(self, *message):
      for txt in message:
        self.st.appendtext(txt)
      self.st.appendtext('\n')
      self.root.update()

    # Save the text in the window
   def saveText(self):
      try:
         filename = tkFileDialog.asksaveasfilename()
         if filename == None: return
         self.st.exportfile(filename)
      except:
         self.Disp()
         self.Disp("*** UNABLE TO SAVE LOG FILE.  ERROR FOLLOWS.")
         self.appendTrace()


    # Since we'd like a little more information about errors, append that bad boy.
   def appendTrace(self):
      self.Disp("Error Type: " , sys.exc_info()[0])
      self.Disp("Error Val : " , sys.exc_info()[1])

    # Oprn the mailbox (external function)
   def selectBox(self):
      try:
         self._selectBox()
         self.Disp()
      except:
         self.Disp("*** UNABLE TO OPEN MESSAGE FILE.  ERROR FOLLOWS.")
         self.appendTrace()

    # Open the mailbox (internal function)
   def _selectBox(self):
       # mBox Strict
      if self.boxtype.get() == self.boxtyps[0]:
         self.mb = mailbox.UnixMailbox (file(self.mailbox.get(),'r'))
         self.Disp(self.boxtype.get(), " at location " ,self.mailbox.get(), " Opened Successfully.")
       # mBox Loose
      elif self.boxtype.get() == self.boxtyps[1]:
         self.mb = mailbox.PortableUnixMailbox(file(self.mailbox.get(),'r'))
         self.Disp(self.boxtype.get(), " at location ", self.mailbox.get(), " Opened Successfully.")
       # MailDir
      elif self.boxtype.get() == self.boxtyps[2]:
         self.mb = mailbox.Maildir(os.path.dirname(self.mailbox.get()))
         self.Disp(self.boxtype.get(), " at location ",os.path.dirname(self.mailbox.get()), " Opened Successfully.")
       # MMDF
      elif self.boxtype.get() == self.boxtyps[3]:
         self.mb = mailbox.MmdfMailbox(file(self.mailbox.get(),'r'))
         self.Disp(self.boxtype.get(), " at location ", self.mailbox.get(), " Opened Successfully.")
       # MH
      elif self.boxtype.get() == self.boxtyps[4]:
         self.mb = mailbox.MHMailbox(file(self.mailbox.get(),'r'))
         self.Disp(self.boxtype.get(), " at location ", self.mailbox.get(), " Opened Successfully.")
       # Babyl
      elif self.boxtype.get() == self.boxtyps[5]:
         self.mb = mailbox.BabylMailbox(file(self.mailbox.get(),'r'))
         self.Disp(self.boxtype.get(), " at location ", self.mailbox.get(), " Opened Successfully.")
       #Unknown File Type
      else:
         self.Disp("*** I don't know about that file type.")
         self.running = 2

    # Feed the Beast (external)
   def process(self):
      try:
         self._process()
      except:
         self.Disp("*** CANNOT READ MESSAGES.  ERROR FOLLOWS.")
         self.appendTrace()

    # Feed the Beast (internal)
   def _process(self):
      self.msg = self.mb.next()
      while self.msg is not None:
         self.processDocument()
         self.msg = self.mb.next()
      self.finishUp()

    # Read Document and then Send It (external)
   def processDocument(self):
      try:
         self._processDocument()
      except:
         self.count[2] = self.count[2] + 1
         self.Disp("*** ", self.count[2] ," MESSAGE READ FAILED, MAY BE MALFORMED.  ERROR FOLLOWS.")
         self.appendTrace()

    # Read Document and then Send It (internal)
   def _processDocument(self):
      self.document = self.msg.fp.read()
      if self.document is not None:
         self.sendMessage()

    # Send a Message (external)
   def sendMessage(self):
      try:
         self._sendMessage()
         self.count[0] = self.count[0] + 1
         self.Disp(self.count[0] ," Forwarded a message from: ", self.msg.getaddr('From')[1])
      except:
         self.count[1] = self.count[1] + 1
         self.Disp("*** ", self.count[1] ," ERROR SENDING MESSAGE FROM: ", self.msg.getaddr('From')[1])
         self.Disp("*** UNABLE TO CONNECT TO SERVER OR SEND MESSAGE. ERROR FOLLOWS.")
         self.appendTrace()

    # Send a Message (Internal)
   def _sendMessage(self):
      time.sleep(2)

      server = smtplib.SMTP(self.smtpsvr.get())
      server.set_debuglevel(1)
      if self.rcvorsn.get() == self.rcvtyps[1]:
         fullmsg = re.sub(r'From: .*', 'From: %s' % self.recipnt.get(),
                          self.msg.__str__( ) + '\x0a' + self.document, 1 )
         server.sendmail(self.recipnt.get(), self.recipnt.get(), fullmsg)
      else:
         fullmsg = self.msg.__str__( ) + '\x0a' + self.document
         server.sendmail(self.msg.getaddr('From')[1], self.recipnt.get(), fullmsg)
      server.quit()

    # Prints the stat information
   def finishUp(self):
      self.Disp()
      self.Disp("Done. Stats: ", self.count[0] ," success ", self.count[1] ," error ", self.count[2] ," skipped.")
      self.running = 0
      self.count = [0,0,0]

    # Performs some last minute testing.
   def execute(self):

      if self.running: return
      else:            self.running = 1

      self.writeHeader()

      if not self.mailbox.get():
         self.Disp("*** You didn't specify a message store.")
         self.running = 2
      if not self.recipnt.get():
         self.Disp("*** You didn't specify a recipient.")
         self.running = 2
      if not self.smtpsvr.get():
         self.Disp("*** You didn't specify a SMTP server.")
         self.running = 2
      if self.needath.get():
         if not self.smtpusr.get():
            self.Disp("*** You didn't specify a SMTP username.")
            self.running = 2
         if not self.smtppwd.get():
            self.Disp("*** You didn't specify a SMTP password.")
            self.running = 2


      self.selectBox()
      self.process()

      if self.running == 2:
         self.running = 0
         self.Disp()
         self.writeInstructions()
         return

      print "Sending messages from", self.mailbox.get(), "to", self.recipnt.get(),
      if self.boxtype.get() == self.boxtyps[2]: print "in Maildir Format",
      else:                  		  print "in mBox Format",
      if self.rcvorsn.get() == self.rcvtyps[1]: print "for sent messages (sender will be rewritten)",
      else:                               print "for recieved messages (sender not rewritten)",
      if self.needath.get(): print "from", self.smtpsvr.get(), "using the username", self.smtpusr.get(), "and password", self.smtppwd.get() + "."
      else:                  print "from", self.smtpsvr.get(), "without authentication."

      # Sometimes you want to see what's in a combo box.  Do this.
      #print self.boxtype.get()


if __name__ == '__main__':

   GmlWin()