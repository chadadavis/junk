#ifndef CHARACTERGRAPHICS_H
#define CHARACTERGRAPHICS_H

// ********************************************************
// * Character Graphics Definition File
// *
// * FILE: charactergraphics.h        
// *
// *
// * PURPOSE: Generic VT100 terminal control functions - 
// *          special font sizing and formatting, manipulating
// *          cursor movement and scroll regions, etc.
// *
// * ACTIONS: Definition of easy to use functions for VT100
// *           control codes.
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

// **************************************************************
// VT100 Support: comment out the applicable following lines if your 
//                terminal doesn't support the control
// **************************************************************
//   DOUBLE: double sized characters will be redefined as normal
//           and double printed code will be printed only once for
//           double high characters
#define VT100_DOUBLE 1
//   FLASH: flashing will be redefined to reverse video
#define VT100_FLASH 1
//#include <iostream.h>


// *******************
// clearing operations
// *******************

// CLEAR SCREEN - clears terminal screen
#define ClearScreen() cout << "\x1b[2J" ;

// CLEAR LINE: clears line cursor is on 
#define ClearLine() cout << "\x1b[2K" ;

// CLEAR TO LEFT: clears from cursor to its left 
#define ClearToLeft() cout << "\x1b[1K" ;

// CLEAR TO RIGHT: clears from cursor to its right
#define ClearToRight() cout << "\x1b[0K" ;

// RESET TERMinal
#define ResetTerm() cout << "\x1b" << 'c' ;


// *******************
// video operations
// *******************

// NORMAL VIDEO: resets to normal video, necessary 
//               after following macros 
#define NormalVideo() cout << "\x1b[0m" ;

// FLASH VIDEO: sets video to flash/blink 
#ifdef VT100_FLASH
#define FlashVideo() cout << "\x1b[5m" ;
#else
#define FlashVideo() cout << "\x1b[7m" ;
#endif

// REVERSE VIDEO: sets to inverse video 
//                (reverse foreground/background)
#define ReverseVideo() cout << "\x1b[7m" ;

// LOW VIDEO: sets video to low intensity
#define LowVideo() cout << "\x1b[2m" ;

// BOLD TEXT: sets characters/video to high intensity
#define BoldText() cout << "\x1b[1m" ;

// UNDERLINE: sets constant underline
#define Underline() cout << "\x1b[4m" ;


// ******************************
// enlarging character operations
// ******************************

// ENLARGE GENERAL: enlarges characters in general
//                  characters are double-width
#ifdef VT100_DOUBLE
#define EnlargeGeneral() cout << "\x1b#6" ;
#else
#define EnlargeGeneral() cout << "\x1b#5" ;
#endif

// ENLARGE TOP: enlarges characters in top half
//              characters are double-width & double height
#ifdef VT100_DOUBLE
#define EnlargeTop() cout << "\x1b#3" ;
#else
#define EnlargeTop() cout << "\x1b#5" ;
#endif

// ENLARGE BOTTOM: enlarges characters in bottom half
//                 characters are double-width & double height
#ifdef VT100_DOUBLE
#define EnlargeBottom() cout << "\x1b#4" ;
#else
#define EnlargeBottom() cout << "\x1b#5" ;
#endif

// DOUBLE TOP: enlarges characters in top half
//             characters are double height & normal width
#ifdef VT100_DOUBLE
#define DoubleTop() cout << "\x1b#:" ;
#else
#define DoubleTop() cout << "\x1b#5" ;
#endif

// DOUBLE BOTTOM: enlarges characters in bottom half
//                characters are double height & normal width
#ifdef VT100_DOUBLE
#define DoubleBottom() cout << "\x1b#;" ;
#else
#define DoubleBottom() cout << "\x1b#5" ;
#endif




// ****************
// cursor movements
// ****************

// HOME CURSOR: sends cursor to [1,1] (upper left)
#define HomeCursor() cout << "\x1b[?6l" ; 

// MOVE CURSOR: moves cursor to position [row, col]
//              alt -- H instead of f
#define MoveCursor(row, col) cout << "\x1b[" << row << ';' << col << 'f' ;

// CURSOR TO LEFT: moves cursor cols columns to the left
#define CursorToLeft(cols) cout << "\x1b[" << cols << 'D' ;

// CURSOR TO RIGHT: moves cursor cols columns to the right
#define CursorToRight(cols) cout << "\x1b[" << cols << 'C' ;

// CURSOR TO LEFT ONE: moves cursor cols columns to the left
#define CursorToLeftOne() cout << "\x1b[D" ;

// CURSOR TO RIGHT: moves cursor cols columns to the right
#define CursorToRightOne() cout << "\x1b[C" ;

// CURSOR UP: moves cursor rows rows upwards
#define CursorUp(rows) cout << "\x1b[" << rows << 'A' ;

// CURSOR DOWN: moves cursor rows rows downwards
#define CursorDown(rows) cout << "\x1b[" << rows << 'B' ;

// to NEXT LINE: moves cursor to beginning of next line
#define NextLine() cout << "\x1b" << 'D' ;       // [1G" ;


// ******************
// screen positioning
// ******************

// SCROLL DOWN: scrolls down 1 row
#define ScrollDown() cout << "\x1bM" ;

// SCROLL UP: scrolls up 1 row
#define ScrollUp() cout << "\x1bD" ;

// SET SCROLL: sets scrolling region
#define SetScroll(topRow, bottomRow) cout << "\x1b[" << topRow << ';' << bottomRow << 'r' ;

// SCROLL CURSOR HOME: takes cursor to ul corner of scrolling region
#define ScrollCursorHome() cout << "\x1b[?6h" ;

// SAVE CURSOR: saves cursor position and attributes
#define SaveCursor() cout << "\x1b" << '7' ;

// RESTORE CURSOR: restores saved cursor position and attributes
#define RestoreCursor() cout << "\x1b" << '8' ;

// *********************
// special character set
// *********************

// SPECIAL CHARACTER SET ON
#define SpecialCharacterSetOn() cout << "\x1b(0" ;

// SPECIAL CHARACTER SET OFF: therefor, normal character set is on
#define SpecialCharacterSetOff() cout << "\x1b(B" ;


// ****
// misc
// ****

// RING BELL
//#define RingBell() cout << '\x07' ;


// ********************
// function prototypes 
// ********************

// UP-LEFT CORNER: puts out an upper-left-hand corner 
void UpLeftCorner() ;

// UP-RIGHT CORNER: puts out an upper-right-hand corner
void UpRightCorner() ;

// LOWER-LEFT CORNER: puts out an lower-left-hand corner
void LowerLeftCorner() ;

// LOWER-RIGHT CORNER: puts out an lower-right-hand corner
void LowerRightCorner() ;

// DRAW HORIZONTAL: draws horiz line to the right from current
//                  position for the number of columns given by param
void DrawHorizontal( int ) ;

// DRAW VERTICAL: draws vertical line downwards from current
//                position for the number of rows given by param
void DrawVertical( int ) ;

// DRAW BOX: draws box from first two parameters (row, col)
//           from upper-left for the total (rows, cols) given
//           by the second two params
void DrawBox( int, int, int, int ) ;

//***************
// misc routines
//***************

// i2a -- converts an integer into a character string

char* i2a  (int n) ; 

// currently not developed 
// i2ch -- converts an integer into a character string
// char  i2ch (int n) ;


// PRINT TIME: prints time in secs out in "ddd days hh:mm:ss" format
//             6 october 1997 - dls

void printTime (int secs) ;


#endif CHARACTERGRAPHICS_H

