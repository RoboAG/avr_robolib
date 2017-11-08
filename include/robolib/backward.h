/******************************************************************************
* robolib/backward.h                                                          *
* ==================                                                          *
*                                                                             *
* Version: 1.0.0                                                              *
* Date   : 27.09.15                                                           *
* Author : Peter Weissig                                                      *
*                                                                             *
* Changelog:                                                                  *
*   27.09.15(V1.0.0)                                                          *
*     - first release                                                         *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/peterweissig/robolib                                   *
******************************************************************************/

#ifndef _ROBOLIB_BACKWARD_H_
#define _ROBOLIB_BACKWARD_H_

//**************************<Macros>*******************************************
//! The number of bytes the types modul uses in order to work
#define ROBOLIB_RAM_COUNT_BACKWARD 0

#ifndef __DOXYGEN__

    // for automated initializing see end of this file

#endif // #ifndef doxygen

//**************************<Included files>***********************************
#include <inttypes.h>
#include <avr/interrupt.h>

//**************************<Types>********************************************

//**************************<Functions>****************************************
// robolib
  //! Initializes all selected moduls.
  #define Init_System() robolib_init()

// string
  //! Sends a string located in the flash.
  #define flashtostr(out,str) string_from_flash(out, str)

  //! Converts a signed 16-bit integer to a string.
  #define int16tostr(out,value,digits) string_from_int(out, value, digits)

  //! Converts an unsigned 16-bit integer to a string.
  #define uint16tostr(out,value,digits) string_from_uint(out, value, digits)

  //! Converts a hexadecimal number to a string.
  #define uint16tohex(out,value,digits) string_from_hex(out, value, digits)

// delay
  //! Waits for ms milliseconds.
  #define mdelay(ms) delay_ms(ms)

// display
  //! Prints data on the display.
  #define lcdout(data) display_print(data)

  //! Clears the display and moves the cursor to position 0,0.
  #define lcdclr() display_clear()

  //! Moves the cursor to a given positon on the display.
  #define lcdxy(x,y) display_gotoxy(x, y)

  //! Changes the appearance of the cursor on the display.
  #define lcdcursor(mode) display_cursor(mode)

  //! Creates an self-defined character.
  #define lcdsetchar(adr, pdata) display_setchar(adr, pdata)

// display_buffer
  //! Prints data into the display buffer.
  #define lcdpout(data) display_buffer_print(data)
  #define lcdpout       display_buffer_print

  //!Clears the virtual display and moves the cursor.
  #define lcdpclr() display_buffer_clear()

  //! Moves the cursor to a given positon on the virtual display.
  #define lcdpxy(x,y) display_buffer_gotoxy(x, y)

  //! Updates the real display with the data from the virtual display.
  #define lcdupdate() display_buffer_update()

  //! Updates the real display with the whole virtual display.
  #define lcdupdateall() display_buffer_updateall()

  #ifndef __DOXYGEN__
      #define lcdout display_print
  #endif

// UART0
  //! Transmits one byte to the UART0.
  #define serout0(data) uart0_send(data)

  //! Recieves one byte from the UART0.
  #define serinp0() uart0_get()

  //! Returns the number of unread bytes of UART0.
  #define serstat0() uart0_rxcount_get()

  #ifndef __DOXYGEN__
      #define serout0 uart0_send
  #endif

// UART1
  //! Transmits one byte to the UART1.
  #define serout1(data) uart1_send(data)

  //! Recieves one byte from the UART1.
  #define serinp1() uart1_get()

  //! Returns the number of unread bytes of UART1.
  #define serstat1() uart1_rxcount_get()

  #ifndef __DOXYGEN__
      #define serout1 uart1_send
  #endif


// automated initializing
#ifndef __DOXYGEN__
    __inline__ void robolib_backward_init_inline(void) {
        #ifdef ROBOLIB_INIT
            ROBOLIB_INIT();
        #endif
    }

    #undef ROBOLIB_INIT
    #define ROBOLIB_INIT()     robolib_backward_init_inline()
    #define robolib_init()     cli(); ROBOLIB_INIT(); sei()
    #define robolib_init_cli() cli(); ROBOLIB_INIT()
#endif

#endif // #ifndef _ROBOLIB_BACKWARD_H_
