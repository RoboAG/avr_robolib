/******************************************************************************
* robolib/tick.h                                                              *
* ==============                                                              *
*                                                                             *
* Version: 1.0.1                                                              *
* Date   : 20.03.18                                                           *
* Author : Peter Weissig                                                      *
*                                                                             *
* Changelog:                                                                  *
*   27.09.15(V1.0.0)                                                          *
*     - first release                                                         *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/RoboAG/avr_robolib                                     *
******************************************************************************/

#ifndef _ROBOLIB_TICK_H_
#define _ROBOLIB_TICK_H_


//**************************<Macros>*******************************************
//! The number of bytes the tick modul uses in order to work
#define ROBOLIB_RAM_COUNT_TICK 0

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
 * The time-period in microseconds is always 10 times
 * higher than useconds.
 * E.g. A value of 20 waits for 200 microseconds.
 *
 * The time-period in milliseconds is truncated to next
 * multiple of 10, due to the intervall of the systick,
 * which is 10 milliseconds.
 * E.g. A value of 283 waits for 270-280 milliseconds.
 *
 * This function is robust to heavy load on interrupts.
 *
 * This function can be called from within an interrupt,
 * but it will enable the global interrupt flag.
 *
 * @param mseconds
 * Time-period in milliseconds.
 *
 * @sa delay_ms()
 */
void tick_delay(uint16_t mseconds);

/**
 * @brief Returns the current systick value.
 *
 * This function returns the current time measured in systick time.
 * One tick is equivalent to 10 milliseconds.
 *
 * The internal systick variable is 16 bits wide. Therefore the highest possible
 * value is 65535 which relates to 10 minutes, 55 seconds and 350 milliseconds.
 *
 * This function can be called from within an interrupt.
 *
 * @return
 * Internal time. [0..65535]
 *
 * @sa tick_tomin(), tick_tosec(), tick_tohsec(), tick_tomsec()
 */
uint16_t tick_get(void);

/**
 * @brief Resets the internal tick counter.
 *
 * This function clears the internal counter variable.
 *
 * This function can be called from within an interrupt.
 *
 * @sa tick_get()
 */
void tick_reset(void);

/**
 * @brief Extracts the minutes from a tic value.
 *
 * This function returns the full minutes from the given tic value.
 *
 * The internal systick variable is 16 bits wide. Therefore the highest possible
 * value is 65535 which relates to 10 minutes, 55 seconds and 350 milliseconds.
 *
 * This function can be called from within an interrupt.
 *
 * @return
 * Minutes. [0..10]
 *
 * @sa tick_get(), tick_tosec(), tick_tohsec(), tick_tomsec()
 */
uint8_t tick_tomin(uint16_t time);

/**
 * @brief Extracts the seconds from a tic value.
 *
 * This function returns the full seconds from the given tic value.
 *
 * The internal systick variable is 16 bits wide. Therefore the highest possible
 * value is 65535 which relates to 10 minutes, 55 seconds and 350 milliseconds.
 *
 * This function can be called from within an interrupt.
 *
 * @return
 * Seconds. [0..59]
 *
 * @sa tick_get(), tick_tomin(), tick_tohsec(), tick_tomsec()
 */
uint8_t tick_tosec(uint16_t time);

/**
 * @brief Extracts the hundredths of a seconds from a tic value.
 *
 * This function returns the fraction of a seconds from the given tic value.
 *
 * The internal systick variable is 16 bits wide. Therefore the highest possible
 * value is 65535 which relates to 10 minutes, 55 seconds and 350 milliseconds.
 *
 * This function can be called from within an interrupt.
 *
 * @return
 * Hundredths of second. [0..99]
 *
 * @sa tick_get(), tick_tomin(), tick_tosec(), tick_tomsec()
 */
uint8_t tick_tohsec(uint16_t time);

/**
 * @brief Extracts the milliseconds from a tic value.
 *
 * This function returns the fraction of a seconds from the given tic value.
 *
 * The internal systick variable is 16 bits wide. Therefore the highest possible
 * value is 65535 which relates to 10 minutes, 55 seconds and 350 milliseconds.
 *
 * This function can be called from within an interrupt.
 *
 * @return
 * Milliseconds [0..990]
 *
 * @sa tick_get(), tick_tomin(), tick_tosec(), tick_tohsec()
 */
uint16_t tick_tomsec(uint16_t time);


/**
 * @brief Systick function.
 *
 * If the systick is used, this function will be called every 10ms.
 *
 * @note This function must be implemented by the user.
 */
__attribute__((weak))
void tick_userfunction(void);

/**
 * @brief Prints general information about the tick.
 *
 * This function prints all the data related to the tick modul.
 *
 * The output is similar to the following: \n
 * @code
 * tick:
 *   tic         : 23085
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
 * @sa robolib_tick_print_compiled()
 */
#ifndef __DOXYGEN__
    void robolib_tick_print(void (*out)(uint8_t));
#else
    void robolib_tick_print(void *out);
#endif

/**
 * @brief Prints information about the version of the tick.
 *
 * This function prints information about the source file(s)
 * of this modul at compile time. This inlcude the version of
 * the main files and important macros.
 *
 * The output is similar to the following: \n
 * @code
 * /source/tick.c 08.09.2014 V0.0.0
 * /source/tick/tick_atmega2561.c 08.09.2014 V0.0.0
 * MCU         : atmega2561
 * F_CPU       : 16000000
 * SYSTICK     : TIMER0
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
 * @sa robolib_tick_print()
 */
#ifndef __DOXYGEN__
    void robolib_tick_print_compiled(void (*out)(uint8_t));
#else
    void robolib_tick_print_compiled(void *out);
#endif


/**
 * @brief Internal function.
 *
 * Do not call this function at all.
 */
void robolib_tick_init(void);

// automated initializing
#ifndef __DOXYGEN__
    __inline__ void robolib_tick_init_inline(void) {
        #ifdef ROBOLIB_INIT
            ROBOLIB_INIT();
        #endif

        robolib_tick_init();
    }

    #undef ROBOLIB_INIT
    #define ROBOLIB_INIT()     robolib_tick_init_inline()
    #define robolib_init()     cli(); ROBOLIB_INIT(); sei()
    #define robolib_init_cli() cli(); ROBOLIB_INIT()
#endif

#endif // #ifndef _ROBOLIB_TICK_H_
