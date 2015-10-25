/******************************************************************************
* system/adc.h                                                                *
* ============                                                                *
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

#ifndef _SYSTEM_ADC_H_
#define _SYSTEM_ADC_H_

//**************************<Macros>*******************************************
//! The number of bytes the adc modul uses in order to work
#define SYSTEM_RAM_COUNT_ADC 8

#ifndef __DOXYGEN__

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
 * This function immediately starts a new adc conversion and returns the result.
 *
 * On error this function returns 65535.
 *
 * This function can be called from within an interrupt,
 * but it will enable the global interrupt flag.
 *
 * @param nr
 * ADC-pin number. [0..7, special modes]
 *
 * @return
 * ADC-value of the given pin. [0..1023, 65535]
 *
 * @sa adc_buffered_get()
 */
uint16_t adc_get(uint8_t nr);

/**
 * @brief Returns the buffered adc-value of the given channel.
 *
 * This function returns the last measured value of the selected channel.
 *
 * On error this function returns 65535.
 *
 * This function can be called from within an interrupt.
 *
 * @param nr
 * ADC-pin number. [0..7]
 *
 * @return
 * ADC-value of the given pin. [0..1023, 65535]
 *
 * @sa adc_get()
 */
uint16_t adc_buffer_get(uint8_t nr);

/**
 * @brief Enables the adc modul.
 *
 * This function enables the whole adc modul.
 *
 * This function can be called from within an interrupt.
 *
 * @sa adc_disable(), adc_is_enabled()
 */
void adc_enable(void);

/**
 * @brief Disables the adc modul.
 *
 * This function disables the whole adc modul.
 *
 * This function can be called from within an interrupt.
 *
 * @sa adc_enable(), adc_is_enabled()
 */
void adc_disable(void);

/**
 * @brief Returns if the adc modul is enabled.
 *
 * This function returns the state of the adc modul.
 *
 * This function can be called from within an interrupt.
 *
 * @return
 * Boolean value. \n
 * False (== 0x00) means the adc is disabled.        \n
 * True  (!= 0x00) means the adc is enabled.         \n
 *
 * @sa adc_enable(), adc_disable()
 */
uint8_t adc_is_enabled(void);

/**
 * @brief Prints general information about the adc.
 *
 * This function prints all the data related to the adc modul.
 *
 * The output is similar to the following: \n
 * @code
 * adc:
 *   enabled     : true
 *   channel[0]  : 564
 *   channel[1]  : 65
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
 * @sa system_adc_print_compiled()
 */
#ifndef __DOXYGEN__
    void system_adc_print(void (*out)(uint8_t));
#else
    void system_adc_print(void* out);
#endif

/**
 * @brief Prints information about the version of the adc.
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
 * @sa system_adc_print()
 */
#ifndef __DOXYGEN__
    void system_adc_print_compiled(void (*out)(uint8_t));
#else
    void system_adc_print_compiled(void* out);
#endif

/**
 * @brief Internal function.
 *
 * Do not call this function at all.
 */
uint8_t system_adc_adcok(void);

/**
 * @brief Internal function.
 *
 * Do not call this function at all.
 */
uint8_t system_adc_getchannel(void);

/**
 * @brief Internal function.
 *
 * Do not call this function at all.
 */
void system_adc_setchannel(uint8_t nr);

/**
 * @brief Internal function.
 *
 * Do not call this function at all.
 */
uint8_t system_adc_savevalue(void);

/**
 * @brief Internal function.
 *
 * Do not call this function at all.
 */
uint8_t system_adc_wait(void);


/**
 * @brief Internal function.
 *
 * Do not call this function at all.
 */
void system_adc_init(void);

// automated initializing
#ifndef __DOXYGEN__
    __inline__ void system_adc_init_inline(void) {
        #ifdef SYSTEM_INIT
            SYSTEM_INIT();
        #endif;

        system_adc_init();
    }

    #define SYSTEM_INIT()     system_adc_init_inline()
    #define system_init()     cli(); SYSTEM_INIT(); sei()
    #define system_init_cli() cli(); SYSTEM_INIT()
#endif

#endif // #ifndef _SYSTEM_ADC_H_
