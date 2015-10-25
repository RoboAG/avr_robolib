/******************************************************************************
* system/display_buffer.h                                                     *
* =======================                                                     *
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

#ifndef _SYSTEM_DISPLAY_BUFFERED_H_
#define _SYSTEM_DISPLAY_BUFFERED_H_


//**************************<Macros>*******************************************
#ifndef __DOXYGEN__
    #ifndef DISPLAY_BUFFER_HEIGHT
        #define DISPLAY_BUFFER_HEIGHT 2
    #endif // ifndef DISPLAY_BUFFER_HEIGHT

    #ifndef DISPLAY_BUFFER_WIDTH
        #define DISPLAY_BUFFER_WIDTH 16
    #endif // ifndef DISPLAY_BUFFER_WIDTH

    // warnings and errors
    #if (DISPLAY_BUFFER_HEIGHT < 1) || (4 < DISPLAY_BUFFER_HEIGHT)
        #error "DISPLAY_BUFFER_HEIGHT must be between 1 and 4"
    #endif // #if (DISPLAY_BUFFER_HEIGHT < 1) || (4 < DISPLAY_BUFFER_HEIGHT)

    #if (DISPLAY_BUFFER_WIDTH < 1) || (16 < DISPLAY_BUFFER_WIDTH)
        #error "DISPLAY_BUFFER_WIDTH must be between 1 and 16"
    #endif // #if (DISPLAY_BUFFER_WIDTH < 1) || (16 < DISPLAY_BUFFER_WIDTH)


    // for automated initializing see end of this file

#endif // #ifndef doxygen

//! The number of bytes the display modul uses in order to work
#define SYSTEM_RAM_COUNT_DISPLAY \
  2 * DISPLAY_BUFFER_WIDTH * DISPLAY_BUFFER_HEIGHT + 1

//**************************<Included files>***********************************
#include <inttypes.h>
#include <avr/interrupt.h>

#include <system/display.h>

//**************************<Types>********************************************

//**************************<Functions>****************************************
/**
 * @brief Prints data into the display buffer.
 *
 * This function transmits one byte of data to the virtual display.
 *
 * This function can be called from within an interrupt.
 *
 * @param data
 * 8-bit data to be printed on the virtual display.
 *
 * @sa display_buffer_clear(), display_buffer_gotoxy(), display_buffer_update()
 */
void display_buffer_print(uint8_t data);

/**
 * @brief Clears the virtual display and moves the cursor.
 *
 * This function clears the virtual display by writing spaces (' ')
 * to each location and it sets the virtual cursor to the home position (0,0)
 * afterwards.
 *
 * This function can be called from within an interrupt.
 *
 * @sa display_buffer_print(), display_buffer_gotoxy(), display_buffer_update()
 */
void display_buffer_clear(void);

/**
 * @brief Moves the cursor to a given positon on the virtual display.
 *
 * This function sets the virtual cursor to the given position (x,y).
 *
 * This function can be called from within an interrupt.
 *
 * @param x
 * Sets the column of the cursor (0..15). \n
 * 0  is the most left  character.        \n
 * The maximum is defined bei DISPLAY_BUFFER_WIDTH
 *
 * @param y
 * Sets the row of the cursor (0..3)      \n
 * 0 is the upper most line.              \n
 * The maximum is defined bei DISPLAY_BUFFER_HEIGHT
 *
 * @sa display_buffer_print(), display_buffer_clear(), display_buffer_update()
 *
 */
void display_buffer_gotoxy(uint8_t x,uint8_t y);

/**
 * @brief Updates the real display with the data from the virtual display.
 *
 * This functions synchonized the display with the virtual display.
 * But only changed characters are printed.
 *
 * If the display is not responding in a resonable amount of time
 * it will be disabled.
 *
 * If the display is disabled this and other display functions will return
 * without any effect.
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @sa display_buffer_print(), display_buffer_updateall()
 */
void display_buffer_update(void);

/**
 * @brief Updates the real display with the whole virtual display.
 *
 * This functions synchonized the whole display with the virtual display.
 * Every character is printed.
 *
 * If the display is not responding in a resonable amount of time
 * it will be disabled.
 *
 * If the display is disabled this and other display functions will return
 * without any effect.
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @sa display_buffer_print(), display_buffer_update()
 */
void display_buffer_updateall(void);

/**
 * @brief Prints general information about the display buffer.
 *
 * This function prints all the data related to the display buffer modul.
 *
 * The output is similar to the following: \n
 * @code
 * display:
 *   x         : 5
 *   y         : 0
 *   buffer[0] : "Display :-)     "
 *   buffer[1] : "####            "
 *   current[0]: "Display :-D     "
 *   current[1]: "###             "
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
 * @sa system_display_print_compiled()
 */
#ifndef __DOXYGEN__
    void system_display_buffer_print(void (*out)(uint8_t));
#else
    void system_display_buffer_print(void* out);
#endif

/**
 * @brief Prints information about the version of the display buffer.
 *
 * This function prints information about the source file(s)
 * of this modul at compile time. This inlcude the version of
 * the main files and important macros.
 *
 * The output is similar to the following: \n
 * @code
 * /source/display_buffer.c 17.10.2014 V0.0.0
 * DISPLAY_BUFFER_HEIGHT: 2
 * DISPLAY_BUFFER_WIDTH : 16
 *    ...
 * @endcode
 *
 * This function passes the resulting string byte by byte to the
 * output function (e.g. display_send()).
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @param out
 * Output function of type "void out(uint8_t)"
 *
 * @sa system_display_print()
 */
#ifndef __DOXYGEN__
    void system_display_buffer_print_compiled(void (*out)(uint8_t));
#else
    void system_display_buffer_print_compiled(void* out);
#endif


/**
 * @brief Internal function.
 *
 * Do not call this function at all.
 */
void system_display_buffer_init(void);

// automated initializing
#ifndef __DOXYGEN__
    __inline__ void system_display_buffer_init_inline(void) {
        #ifdef SYSTEM_INIT
            SYSTEM_INIT();
        #endif;

        system_display_buffer_init();
    }

    #define SYSTEM_INIT()     system_display_buffer_init_inline()
    #define system_init()     cli(); SYSTEM_INIT(); sei()
    #define system_init_cli() cli(); SYSTEM_INIT()
#endif

#endif // #ifndef _SYSTEM_DISPLAY_BUFFERED_H_
