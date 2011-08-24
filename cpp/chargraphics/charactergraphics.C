// ********************************************************
// * Character Graphics Definition File
// *
// * FILE: charactergraphics.C          
// *
// *
// * PURPOSE: Generic VT100 terminal control functions - 
// *          special font sizing and formatting, manipulating
// *          cursor movement and scroll regions, etc.
// *
// * ACTIONS: Definition of easy to use functions for VT100
// *           control codes (#defines in header file).
// *          Functions for draw lines and boxes.
// *          Convert an integer to a character string.
// *
// *
// * Demo 1 Team 3
// *   dls9 jes25 ted2 mfs2       CSE 249          6 October 1997
// *
// *          
// * Author: mel neville         - mkn  - original 
//           deborah lee soltesz - dls9 - 5 september 1997
// *
// * History:
// *          printTime:
// *            deborah lee soltesz - dls9 - 6 october 1997
// *
// *********************************************************
#include <string.h>
#include <iostream.h>
#include "charactergraphics.h"


// ********************
// function definitions 
// ********************

// UP-LEFT CORNER: puts out an upper-left-hand corner

void UpLeftCorner( void ) {
  SpecialCharacterSetOn() ;
  cout << 'l' ;
  SpecialCharacterSetOff() ;
}


// UP-RIGHT CORNER: puts out an upper-right-hand corner

void UpRightCorner( void ) {
  SpecialCharacterSetOn();
  cout << 'k' ;
  SpecialCharacterSetOff() ;
}


// LOWER-LEFT CORNER: puts out an lower-left-hand corner

void LowerLeftCorner( void ) {
  SpecialCharacterSetOn();
  cout << 'm' ;
  SpecialCharacterSetOff() ;
}


// LOWER-RIGHT CORNER: puts out an lower-right-hand corner

void LowerRightCorner( void ) {
  SpecialCharacterSetOn();
  cout << 'j' ;
  SpecialCharacterSetOff() ;
}


// DRAW HORIZONTAL: draws horiz line to the right from current
//                  position for the number of columns given by param
// CORRECT? why doesn't cursor need to be moved each iteration?

void DrawHorizontal( int numcols ) {
  int col ;
  SpecialCharacterSetOn();
  for (col = 1 ; col <= numcols ; col++) 
    cout << 'q' ;
  SpecialCharacterSetOff() ;
}


// DRAW VERTICAL: draws vertical line downwards from current
//                position for the number of rows given by param

void DrawVertical( int numrows ) {
  int row ;
  SpecialCharacterSetOn();
  for (row = 1 ; row <= numrows ; row++) {
    cout << 'x' ;
    CursorToLeft(1) ;
    CursorDown(1) ;
  }
  SpecialCharacterSetOff() ;
}


// DRAW BOX: draws box from first two parameters (row, col)
//           from upper-left for the total (rows, cols) given
//           by the second two params

void DrawBox( int up_left_row, int up_left_col,
              int num_rows,    int num_cols ) {

  MoveCursor( up_left_row, up_left_col) ;
  UpLeftCorner() ;
  DrawHorizontal (num_cols - 2) ;
  UpRightCorner() ;
  CursorToLeft(1) ;
  CursorDown(1) ;
  DrawVertical (num_rows - 2) ;
  MoveCursor (up_left_row, up_left_col ) ;
  CursorDown(1) ;
  DrawVertical( num_rows - 2 ) ;
  LowerLeftCorner() ;
  DrawHorizontal( num_cols - 2 ) ;
  LowerRightCorner() ;
}

//***************
// handy routines
//***************

// i2a -- converts a positive integer into a character string, and returns 
//        a pointer to the string

char* i2a (int n) {
  char* str ;
  int temp = n ;
  int i = 0 ;

  while (temp) {
    temp /= 10 ;
    i++ ;
  }

  str = new char[i + 1] ;
  str[i] = '\0' ;
  temp = n ;
  
  while (temp && i) {
    str[i - 1] = (48 + temp % 10) ;
    temp /=10 ;
    i-- ;
  }

  return str ;

}



// PRINT TIME: prints time in secs out in "ddd days hh:mm:ss" format
//             6 october 1997 - dls

void printTime (int secs) {

   if ( secs > (24 * 60 * 60) ) {
     cout << secs / (24 * 60 * 60) << " days " ;
     secs = secs % (24 * 60 * 60) ;
   }

   int hour10 = (secs / ( 60 * 60 )) / 10 ;
   int hour1  = (secs / ( 60 * 60 )) % 10 ;
   int min10  = ( (secs / 60) % 60 ) / 10 ;
   int min1   = ( (secs / 60) % 60 ) % 10 ;
   int secs10 = (secs % 60 ) / 10 ;
   int secs1  = (secs % 60 ) % 10 ;

   cout << hour10 << hour1 << ':'
        << min10  << min1  << ':'
        << secs10 << secs1 << flush ; 

}




