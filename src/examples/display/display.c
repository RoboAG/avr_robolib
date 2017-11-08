/******************************************************************************
* examples/display.c                                                          *
* ==================                                                          *
*                                                                             *
* Version: 1.0.0                                                              *
* Date   : 27.09.15                                                           *
* Author : Peter Weissig                                                      *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/peterweissig/robolib                                   *
******************************************************************************/

// include from gcc
#include <inttypes.h>

// include all necessary headers from robolib
#include "display_header.h"

int main (void) {
  // initialize robolib
  robolib_init();

  // draw a smiley on the Display
  display_clear();
  string_from_const(display_print, "Display :-)");

  // variable fo storing last position of "*"
  uint8_t position = 0;

  // endless loop
  while (1) {
    // change the smiley
    display_gotoxy(10,0);
    display_print('D');

    // wait for 0.25 seconds
    delay_ms(500);

    // change the smiley
    display_gotoxy(10,0);
    display_print('|');

    // wait for 0.25 seconds
    delay_ms(500);


    // clear old "*"
    display_gotoxy(position,1);
    display_print(' ');

    // calculate new position
    ++position;
    if (position > 15) {position = 0;}

    // draw new "*"
    display_gotoxy(position,1);
    display_print('*');
  }

  return (0);
}


/******************************************************************************
*                                                                             *
* How to compile this example                                                 *
* ===========================                                                 *
*                                                                             *
* required files                                                              *
*   + display.c             (source code, this file)                          *
*   + display_header.ini    (headerfile as '.ini')                            *
*   + Makefile              (needed for compiling, linking and downloading)   *
*                                                                             *
* 1. adjust Makefile                              (only needed once)          *
*   + set "MCU" to the controller type              (e.g. atmega64)           *
*   + set "PATH_LIB" to the library                 (e.g. ~/avr/robolib)       *
*                                                                             *
* 2. compile library and create headerfile        (only needed once)          *
*   + open a console                                (e.g. strg+alt+t)         *
*   + change to the directory of the files          (e.g. cd ~/avr/test/)     *
*   + run make with "ini" as parameter              (e.g. make ini)           *
*     ==> check output for errors                                             *
*                                                                             *
* 3. compile main file and link everything        (needed for all changes)    *
*   + open a console                                (e.g. strg+alt+t)         *
*   + change to the directory of the files          (e.g. cd ~/avr/test/)     *
*   + run make with "all" as parameter              (e.g. make all)           *
*     ==> check output for errors                                             *
*                                                                             *
* How to download this example                                                *
* ============================                                                *
*                                                                             *
* required files                                                              *
*   + out.hex          (machine code, created in the steps above)             *
*   + Makefile         (needed for compiling, linking and downloading)        *
*                                                                             *
* 1. adjust Makefile                              (only needed once)          *
*   + set "COMPORT_PROG" to the serial device       (e.g. /dev/ttyS0)         *
*   + set "BAUDRATE_PROG" to the speed              (e.g. 57600)              *
*                                                                             *
* 2. download result                              (needed for all changes)    *
*   + open a console                                (e.g. strg+alt+t)         *
*   + change to the directory of the files          (e.g. cd ~/avr/test/)     *
*   + run make with "program" as parameter          (e.g. make program)       *
*     ==> check output for errors                                             *
*                                                                             *
******************************************************************************/
