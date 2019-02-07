/******************************************************************************
* examples/tick.c                                                             *
* ===============                                                             *
*                                                                             *
* Version: 1.0.0                                                              *
* Date   : 27.09.15                                                           *
* Author : Peter Weissig                                                      *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/RoboAG/avr_robolib                                     *
******************************************************************************/

// include from gcc
#include <inttypes.h>

// include all necessary headers from robolib
#include "tick.h"

int main (void) {
  // initialize robolib
  robolib_init();

  // endless loop
  while (1) {
    // current time
    uint16_t time;
    uint8_t  minutes;
    uint8_t  seconds;

    // update current time
    time = tick_get();
    minutes = tick_tomin(time);
    seconds = tick_tosec(time);


    // print result to uart0
    string_from_const(uart0_send, "current time - ");
    string_from_uint (uart0_send, minutes, 2);
    string_from_const(uart0_send, ":");
    string_from_uint (uart0_send, seconds, 2);
    string_from_const(uart0_send, "\r\n");

    // wait for 0.25 seconds
    tick_delay(250);
  }

  return (0);
}


/******************************************************************************
*                                                                             *
* How to compile this example                                                 *
* ===========================                                                 *
*                                                                             *
* required files                                                              *
*   + tick.c                (source code, this file)                          *
*   + tick_header.ini       (headerfile as '.ini')                            *
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
