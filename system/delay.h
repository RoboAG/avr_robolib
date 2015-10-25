/******************************************************************************
* system/delay.h                                                              *
* ==============                                                              *
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

#ifndef _SYSTEM_DELAY_H_
#define _SYSTEM_DELAY_H_


//**************************<Macros>*******************************************
//! The number of bytes the delay modul uses in order to work
#define SYSTEM_RAM_COUNT_DELAY 0

#ifndef __DOXYGEN__

    // warnings and errors
    #ifndef F_CPU
        #error "F_CPU must be set (e.g. 16MHz: 16000000)"
    #endif // #ifndef F_CPU


    // for automated initializing see end of this file

#endif // #ifndef doxygen

//**************************<Included files>***********************************
#include <inttypes.h>
#include <avr/interrupt.h>

//**************************<Types>********************************************

//**************************<Functions>****************************************
/**
 * @brief Waits for mseconds milliseconds.
 *
 * This function executes a certain number of commands within a loop.
 * If there is a heavy load on interrupts the duration will increase
 * significant.
 *
 * This function can be called from within an interrupt.
 *
 * @param mseconds
 * Time-period in milliseconds.
 *
 * @sa delay_us(), timer_delay()
 */
void delay_ms(uint16_t mseconds);

# if !defined(__DOXYGEN__)
    /* Frome file: /util/delay_basic.h
       Copyright (c) 2002, Marek Michalkiewicz
       Copyright (c) 2007 Joerg Wunsch
    */
    void loopdelay(uint8_t __count);
# endif // if !defined(__DOXYGEN__)

/**
 * @brief Waits for 10x useconds microseconds.
 *
 * The time-period in microseconds is always 10 times
 * higher than useconds.
 * E.g. A value of 20 waits for 200 microseconds.
 *
 * This function executes a certain number of commands within a loop.
 * If there is a heavy load on interrupts the duration will increase
 * significant.
 *
 * This function can be called from within an interrupt.
 *
 * @param useconds
 * Time-period in 10x microseconds. \n
 *
 * @sa delay_ms()
 */
void delay_us(uint16_t useconds);

/**
 * @brief Prints general information about the delay.
 *
 * This function prints all the data related to the delay modul.
 *
 * The output is similar to the following: \n
 * @code
 * delay:
 *   [empty]
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
 * @sa system_delay_print_compiled()
 */
#ifndef __DOXYGEN__
    void system_delay_print(void (*out)(uint8_t));
#else
    void system_delay_print(void* out);
#endif

/**
 * @brief Prints information about the version of the delay.
 *
 * This function prints information about the source file(s)
 * of this modul at compile time. This inlcude the version of
 * the main files and important macros.
 *
 * The output is similar to the following: \n
 * @code
 * /source/delay.c 09.09.2014 V0.0.0
 * F_CPU       : 16000000
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
 * @sa system_delay_print()
 */
#ifndef __DOXYGEN__
    void system_delay_print_compiled(void (*out)(uint8_t));
#else
    void system_delay_print_compiled(void* out);
#endif


// automated initializing
#ifndef __DOXYGEN__
    __inline__ void system_delay_init_inline(void) {
        #ifdef SYSTEM_INIT
            SYSTEM_INIT();
        #endif;
    }

    #define SYSTEM_INIT()     system_delay_init_inline()
    #define system_init()     cli(); SYSTEM_INIT(); sei()
    #define system_init_cli() cli(); SYSTEM_INIT()
#endif

#endif // #ifndef _SYSTEM_DELAY_H_
