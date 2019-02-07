/******************************************************************************
* robolib/adc_timer.h                                                         *
* ===================                                                         *
*                                                                             *
* Version: 0.0.0                                                              *
* Date   : 27.09.15                                                           *
* Author : Peter Weissig                                                      *
*                                                                             *
* Changelog:                                                                  *
*   27.09.15(V0.0.0)                                                          *
*     - first release                                                         *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/RoboAG/avr_robolib                                     *
******************************************************************************/

#ifndef _ROBOLIB_ADC_TIMER_H_
#define _ROBOLIB_ADC_TIMER_H_

//**************************<Macros>*******************************************
//! The number of bytes the adc modul uses in order to work
#define ROBOLIB_RAM_COUNT_ADC_TIMER 2

#ifndef __DOXYGEN__

    // warnings and errors
    #ifdef ROBOLIB_SYSTICK
        #error "robolib/timer.h" has to be included after this file
    #endif //#ifndef ROBOLIB_SYSTICK


    // for automated systick implementation see end of this file

    // for automated initializing see end of this file

#endif // #ifndef doxygen

//**************************<Included files>***********************************
#include <inttypes.h>
#include <avr/interrupt.h>

//**************************<Types>********************************************

//**************************<Functions>****************************************
/**
 * @brief Returns the adc-value of the given channel.
 *
 * This function returns the last measured value of the given adc-channel.
 *
 * This function can be called from within an interrupt.
 *
 * @param nr
 * ADC-pin number. [0..7]
 *
 * @return
 * ADC-value of the given pin. [0..1023]
 *
 * @sa adc_timer_mask_set(), adc_timer_get_direct()
 */
#ifndef __DOXYGEN__
    #define adc_timer_get(nr) adc_buffer_get(nr)
#else
    uint16_t adc_timer_get(uint8_t nr);
#endif

/**
 * @brief Changes the bit-mask of the adc.
 *
 * This function selects which adc channels are measured.                    \n
 * For each set bit (== 1) the corresponding pin will be
 * continuously measured.                                                    \n
 * For each cleard bit (== 0) the corresponding pin will not be considered.  \n
 *
 * This function can be called from within an interrupt.
 *
 * @param mask
 * Bit mask selecting the ADC-pins.
 *
 * @sa adc_timer_mask_set(), adc_timer_pin_add(), adc_timer_pin_remove()
 */
void adc_timer_mask_set(uint8_t mask);

/**
 * @brief Returns the bit-mask of the adc.
 *
 * This function returns the adc-mask.                                       \n
 * For each set bit (== 1) the corresponding pin is continuously measured.   \n
 * For each cleard bit (== 0) the corresponding pin is not considered.       \n
 *
 * This function can be called from within an interrupt.
 *
 * @return
 * Bit mask with the selected ADC-pins.
 *
 * @sa adc_timer_mask_set(), adc_timer_pin_add(),
 *     adc_timer_pin_remove()
 */
uint8_t adc_timer_mask_get(void);

/**
 * @brief Adds a pin to the adc mask.
 *
 * This function adds the selected pin to the adc mask. \n
 * The selected pin will be continuously measured.
 *
 * This function can be called from within an interrupt.
 *
 * @param nr
 * ADC-pin number. [0..7]
 *
 * @sa adc_timer_pin_remove(), adc_timer_mask_set(), adc_timer_mask_get()
 */
void adc_timer_pin_add(uint8_t nr);

/**
 * @brief Removes a pin from the adc mask.
 *
 * This function removes the selected pin from the adc mask. \n
 * The selected pin will not be measured.
 *
 * This function can be called from within an interrupt.
 *
 * @param nr
 * ADC-pin number. [0..7]
 *
 * @sa adc_timer_pin_add(), adc_timer_mask_set(), adc_timer_mask_get()
 */
void adc_timer_pin_remove(uint8_t nr);

/**
 * @brief Prints general information about the adc timer.
 *
 * This function prints all the data related to the adc timer modul.
 *
 * The output is similar to the following: \n
 * @code
 * adc timer:
 *   mask        : 0b00011110
 *   next        : 3
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
 * @sa robolib_adc_timer_print_compiled()
 */
#ifndef __DOXYGEN__
    void robolib_adc_timer_print(void (*out)(uint8_t));
#else
    void robolib_adc_timer_print(void* out);
#endif

/**
 * @brief Prints information about the version of the adc timer.
 *
 * This function prints information about the source file(s)
 * of this modul at compile time. This inlcude the version of
 * the main files and important macros.
 *
 * The output is similar to the following: \n
 * @code
 * /source/adc.c 26.08.2014 V0.0.0
 * /source/adc/adc_atmega2561.c 26.08.2014 V0.0.0
 * MCU         : atmega2561
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
 * @sa robolib_adc_timer_print()
 */
#ifndef __DOXYGEN__
    void robolib_adc_timer_print_compiled(void (*out)(uint8_t));
#else
    void robolib_adc_timer_print_compiled(void* out);
#endif


/**
 * @brief Internal function.
 *
 * Do not call this function at all.
 */
void robolib_adc_timer_tick(void);

// automated systick implementation
#ifndef __DOXYGEN__
    __inline__ void robolib_adc_timer_tick_inline(void) {
        #ifdef ROBOLIB_TICK_FUNCTION
            ROBOLIB_TICK_FUNCTION();
        #endif

        robolib_adc_timer_tick();
    }

    #define ROBOLIB_TICK_FUNCTION() robolib_adc_timer_tick_inline()
#endif


/**
 * @brief Internal function.
 *
 * Do not call this function at all.
 */
void robolib_adc_timer_init(void);

// automated initializing
#ifndef __DOXYGEN__
    __inline__ void robolib_adc_timer_init_inline(void) {
        #ifdef ROBOLIB_INIT
            ROBOLIB_INIT();
        #endif

        robolib_adc_timer_init();
    }

    #undef ROBOLIB_INIT
    #define ROBOLIB_INIT()     robolib_adc_timer_init_inline()
    #define robolib_init()     cli(); ROBOLIB_INIT(); sei()
    #define robolib_init_cli() cli(); ROBOLIB_INIT()
#endif

#endif // #ifndef _ROBOLIB_ADC_TIMER_H_
