/******************************************************************************
* robolib/display.h                                                           *
* =================                                                           *
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

#ifndef _ROBOLIB_DISPLAY_H_
#define _ROBOLIB_DISPLAY_H_


//**************************<Macros>*******************************************
#ifndef __DOXYGEN__

    //
    // display port for signals
    //
    #ifndef DISPLAY_PORT
        #define DISPLAY_PORT A
    #endif // ifndef DISPLAY_PORT

    #define ROBOLIB_DISPLAY_PORT ROBOLIB_PORT_CREATE(DISPLAY_PORT)
    #define ROBOLIB_DISPLAY_PIN  ROBOLIB_PIN_CREATE( DISPLAY_PORT)
    #define ROBOLIB_DISPLAY_DDR  ROBOLIB_DDR_CREATE( DISPLAY_PORT)

    #ifndef DISPLAY_PIN_E
        #define DISPLAY_PIN_E  4
    #endif // #ifndef DISPLAY_PIN_E

    #ifndef DISPLAY_PIN_RS
        #define DISPLAY_PIN_RS 5
    #endif // #ifndef DISPLAY_PIN_RS

    #ifndef DISPLAY_PIN_RW
        #define DISPLAY_PIN_RW 6
    #endif // #ifndef DISPLAY_PIN_RW

    //
    // display port for data
    //
    #ifndef DISPLAY_DATA_PORT
        #define DISPLAY_DATA_PORT DISPLAY_PORT
    #endif // ifndef DISPLAY_PORT

    #define ROBOLIB_DISPLAY_DATA_PORT ROBOLIB_PORT_CREATE(DISPLAY_DATA_PORT)
    #define ROBOLIB_DISPLAY_DATA_PIN  ROBOLIB_PIN_CREATE( DISPLAY_DATA_PORT)
    #define ROBOLIB_DISPLAY_DATA_DDR  ROBOLIB_DDR_CREATE( DISPLAY_DATA_PORT)

    #ifdef DISPLAY_DATA_HI
        #define ROBOLIB_DISPLAY_BUSY_GET()    (ROBOLIB_DISPLAY_DATA_PIN &  0x80)
        #define ROBOLIB_DISPLAY_DATA_DDR_R()  (ROBOLIB_DISPLAY_DATA_DDR&= ~0xF0)
        #define ROBOLIB_DISPLAY_DATA_DDR_W()  (ROBOLIB_DISPLAY_DATA_DDR|=  0xF0)
        #define ROBOLIB_DISPLAY_DATA_GET_L()  (ROBOLIB_DISPLAY_DATA_PIN >>    4)
        #define ROBOLIB_DISPLAY_DATA_GET_H()  (ROBOLIB_DISPLAY_DATA_PIN &  0xF0)
        #define ROBOLIB_DISPLAY_DATA_SET_L(x) (ROBOLIB_DISPLAY_DATA_PORT = \
          (ROBOLIB_DISPLAY_DATA_PORT & 0x0F) |(x <<   4))
        #define ROBOLIB_DISPLAY_DATA_SET_H(x) (ROBOLIB_DISPLAY_DATA_PORT = \
          (ROBOLIB_DISPLAY_DATA_PORT & 0x0F) |(x & 0xF0))
        #define ROBOLIB_DISPLAY_DATA_SET_0()  (ROBOLIB_DISPLAY_DATA_PORT = \
          (ROBOLIB_DISPLAY_DATA_PORT & 0x0F))
    #else // #ifdef DISPLAY_DATA_HI
        #define ROBOLIB_DISPLAY_BUSY_GET()    (ROBOLIB_DISPLAY_DATA_PIN &  0x08)
        #define ROBOLIB_DISPLAY_DATA_DDR_R()  (ROBOLIB_DISPLAY_DATA_DDR&= ~0x0F)
        #define ROBOLIB_DISPLAY_DATA_DDR_W()  (ROBOLIB_DISPLAY_DATA_DDR|=  0x0F)
        #define ROBOLIB_DISPLAY_DATA_GET_L()  (ROBOLIB_DISPLAY_DATA_PIN &  0x0F)
        #define ROBOLIB_DISPLAY_DATA_GET_H()  (ROBOLIB_DISPLAY_DATA_PIN <<    4)
        #define ROBOLIB_DISPLAY_DATA_SET_L(x) (ROBOLIB_DISPLAY_DATA_PORT = \
          (ROBOLIB_DISPLAY_DATA_PORT & 0xF0) |(x & 0x0F))
        #define ROBOLIB_DISPLAY_DATA_SET_H(x) (ROBOLIB_DISPLAY_DATA_PORT = \
          (ROBOLIB_DISPLAY_DATA_PORT & 0xF0) |(x >>   4))
        #define ROBOLIB_DISPLAY_DATA_SET_0()  (ROBOLIB_DISPLAY_DATA_PORT = \
          (ROBOLIB_DISPLAY_DATA_PORT & 0xF0))
    #endif // #ifdef DISPLAY_DATA_HI

    #define ROBOLIB_DISPLAY_PIN_SET_E(x)  (x ? (ROBOLIB_DISPLAY_PORT|= \
      _BV(DISPLAY_PIN_E )): (ROBOLIB_DISPLAY_PORT&= ~_BV(DISPLAY_PIN_E )))
    #define ROBOLIB_DISPLAY_PIN_SET_RS(x) (x ? (ROBOLIB_DISPLAY_PORT|= \
      _BV(DISPLAY_PIN_RS)): (ROBOLIB_DISPLAY_PORT&= ~_BV(DISPLAY_PIN_RS)))
    #define ROBOLIB_DISPLAY_PIN_SET_RW(x) (x ? (ROBOLIB_DISPLAY_PORT|= \
      _BV(DISPLAY_PIN_RW)): (ROBOLIB_DISPLAY_PORT&= ~_BV(DISPLAY_PIN_RW)))

    // warnings and errors
    #if (DISPLAY_PIN_E  < 0) || (7 < DISPLAY_PIN_E )
        #error "DISPLAY_PIN_E must be between 0 and 7"
    #endif // #if (DISPLAY_PIN_E  < 0) || (7 < DISPLAY_PIN_E )

    #if (DISPLAY_PIN_RS < 0) || (7 < DISPLAY_PIN_RS)
        #error "DISPLAY_PIN_RS must be between 0 and 7"
    #endif // #if (DISPLAY_PIN_RS < 0) || (7 < DISPLAY_PIN_RS)

    #if (DISPLAY_PIN_RW < 0) || (7 < DISPLAY_PIN_RW)
        #error "DISPLAY_PIN_RW must be between 0 and 7"
    #endif // #if (DISPLAY_PIN_Rw < 0) || (7 < DISPLAY_PIN_RW)

    #if DISPLAY_PIN_E  == DISPLAY_PIN_RS
        #error "DISPLAY_PIN_E  and DISPLAY_PIN_RS must not be euqal"
    #endif // #if DISPLAY_PIN_E  = DISPLAY_PIN_RS

    #if DISPLAY_PIN_E  == DISPLAY_PIN_RW
        #error "DISPLAY_PIN_E  and DISPLAY_PIN_RW must not be euqal"
    #endif // #if DISPLAY_PIN_E  = DISPLAY_PIN_RW

    #if DISPLAY_PIN_RS == DISPLAY_PIN_RW
        #error "DISPLAY_PIN_RS and DISPLAY_PIN_RW must not be euqal"
    #endif // #if DISPLAY_PIN_RS = DISPLAY_PIN_RW


    // for automated initializing see end of this file

#endif // #ifndef doxygen

//! The number of bytes the display modul uses in order to work
#define ROBOLIB_RAM_COUNT_DISPLAY 1

//**************************<Included files>***********************************
#include <inttypes.h>
#include <avr/interrupt.h>

#include <robolib/delay.h>
#include <robolib/assembler.h>

//**************************<Types>********************************************

//**************************<Functions>****************************************
/**
 * @brief Prints data on the display.
 *
 * This function transmits one byte of data to the hardware buffer
 * of the connected display.
 *
 * If the display is not responding in a resonable amount of time
 * it will be disabled.
 *
 * If the display is disabled this and other display functions will return
 * without any effect.
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @param data
 * 8-bit data to be printed on the display.
 *
 * @sa display_clear(), display_gotoxy(), display_cursor(), display_setchar()
 */
void display_print(uint8_t data);

/**
 * @brief Clears the display and moves the cursor to position 0,0.
 *
 * This function clears the hardware buffer of the connected display
 * by writing spaces (' ') to each location and it sets the cursor to
 * the home position (0,0) afterwards.
 *
 * If the display is not responding in a resonable amount of time
 * it will be disabled.
 *
 * If the display is disabled this and other display functions will return
 * without any effect.
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @sa display_print(), display_gotoxy(), display_home()
 */
void display_clear(void);

/**
 * @brief Moves the cursor to position 0,0 on the display.
 *
 * This function sets the cursor to the home position (0,0).
 *
 * If the display is not responding in a resonable amount of time
 * it will be disabled.
 *
 * If the display is disabled this and other display functions will return
 * without any effect.
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @sa display_print(), display_clear(), display_gotoxy()
 */
void display_home(void);

/**
 * @brief Moves the cursor to a given positon on the display.
 *
 * This function sets the cursor to the given position (x,y).
 *
 * If the display is not responding in a resonable amount of time
 * it will be disabled.
 *
 * If the display is disabled this and other display functions will return
 * without any effect.
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @param x
 * Sets the column of the cursor (0..15). \n
 * 0  is the most left  character.        \n
 * 15 is the most right character.
 *
 * @param y
 * Sets the row of the cursor (0..3)      \n
 * 0 is the upper most line.              \n
 * 3 is the lowest line.
 *
 * @sa display_print(), display_clear(), display_home(), display_cursor()
 *
 */
void display_gotoxy(uint8_t x,uint8_t y);

/**
 * @brief Changes the appearance of the cursor on the display.
 *
 * This function sets the cursor style.
 *
 * If the display is not responding in a resonable amount of time
 * it will be disabled.
 *
 * If the display is disabled this and other display functions will return
 * without any effect.
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @param mode
 * * 0 - the cursor is invisible
 * * 1 - the whole character is blinking
 * * 2 - the cursor is visible
 * * 3 - the cursor is blinking
 *
 * @sa display_gotoxy(), display_clear(), display_home(), display_print()
 */
void display_cursor(uint8_t mode);

/**
 * @brief Creates an self-defined character.
 *
 * This function changes the selected character.
 *
 * The display supports 8 self-defined characters (0x00 - 0x07).
 * Each characters size is 5x8 pixels including the lowest row for the cursor.
 * The state of every single pixel has to be defined.
 * One row of pixels is packed into one byte starting at the bottom.
 * The lowest bit of each byte relates to the most right pixel and
 * the 4th bit relates to the most left pixel.
 *
 * @verbatim
 * uint8_t rows[8];
 * rows[7] = 0b00000; //
 * rows[6] = 0b01010; //   X   X
 * rows[5] = 0b00000; //
 * rows[4] = 0b00100; //     X
 * rows[3] = 0b00100; //     X
 * rows[2] = 0b10001; // X       X
 * rows[1] = 0b01110; //   X X X
 * rows[0] = 0b00000; // reserved for the cursor
 * @endverbatim
 *
 * If the display is not responding in a resonable amount of time
 * it will be disabled.
 *
 * If the display is disabled this and other display functions will return
 * without any effect.
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @param adr
 * 8-bit integer which selects the character (0..7).
 *
 * @param data
 * Pointer to an array of 8 bytes located in the RAM.
 *
 * @sa display_print(), display_gotoxy()
 */
void display_setchar(uint8_t adr, uint8_t *data);

/**
 * @brief Enables the display modul.
 *
 * This function enables the whole display modul.
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @sa display_is_enabled(), display_disable()
 */
void display_enable(void);

/**
 * @brief Disables the display modul.
 *
 * This function disables the whole display modul.
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @sa display_is_enabled(), display_enable()
 */
void display_disable(void);

/**
 * @brief Returns if the display modul is enabled.
 *
 * This function returns the state of the display modul.
 *
 * This function can be called from within an interrupt.
 *
 * @return
 * Boolean value. \n
 * False (== 0x00) means the display is disabled.        \n
 * True  (!= 0x00) means the display is enabled.         \n
 *
 * @sa display_enable(), display_disable()
 */
uint8_t display_is_enabled(void);

/**
 * @brief Prints general information about the display.
 *
 * This function prints all the data related to the display modul.
 *
 * The output is similar to the following: \n
 * @code
 * display:
 *   enabled     : true
 *    ...
 * @endcode
 *
 * This function passes the resulting string byte by byte to the
 * output function (e.g. uart0_send()).
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @param out
 * Output function of type "void out(uint8_t)"
 *
 * @sa robolib_display_print_compiled()
 */
#ifndef __DOXYGEN__
    void robolib_display_print(void (*out)(uint8_t));
#else
    void robolib_display_print(void* out);
#endif

/**
 * @brief Prints information about the version of the display.
 *
 * This function prints information about the source file(s)
 * of this modul at compile time. This includes the version of
 * the main files and important macros.
 *
 * The output is similar to the following: \n
 * @code
 * /source/display.c 16.10.2014 V0.0.0
 * DISPLAY_PORT      : PORTA
 * DISPLAY_PIN_E     : 4
 * DISPLAY_PIN_RS    : 5
 * DISPLAY_PIN_RW    : 6
 * DISPLAY_DATA_PORT : PORTA
 * DISPLAY_DATA_HI   : false
 *    ...
 * @endcode
 *
 * This function passes the resulting string byte by byte to the
 * output function (e.g. uart_send()).
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @param out
 * Output function of type "void out(uint8_t)"
 *
 * @sa robolib_display_print()
 */
#ifndef __DOXYGEN__
    void robolib_display_print_compiled(void (*out)(uint8_t));
#else
    void robolib_display_print_compiled(void* out);
#endif


/**
 * @brief Internal function.
 *
 * Do not call this function at all.
 */
void robolib_display_init(void);

// automated initializing
#ifndef __DOXYGEN__
    __inline__ void robolib_display_init_inline(void) {
        #ifdef ROBOLIB_INIT
            ROBOLIB_INIT();
        #endif

        robolib_display_init();
    }

    #undef ROBOLIB_INIT
    #define ROBOLIB_INIT()     robolib_display_init_inline()
    #define robolib_init()     cli(); ROBOLIB_INIT(); sei()
    #define robolib_init_cli() cli(); ROBOLIB_INIT()
#endif

#endif // #ifndef _ROBOLIB_DISPLAY_H_
