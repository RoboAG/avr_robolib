/******************************************************************************
* robolib/eeprom.h                                                            *
* ================                                                            *
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

#ifndef _ROBOLIB_EEPROM_H_
#define _ROBOLIB_EEPROM_H_


//**************************<Macros>*******************************************
#ifndef __DOXYGEN__

    // for automated initializing see end of this file

#endif // #ifndef doxygen

//! The number of bytes the eeprom modul uses in order to work
#define ROBOLIB_RAM_COUNT_EEPROM 2

//**************************<Included files>***********************************
#include <inttypes.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>

//**************************<Types>********************************************

//**************************<Functions>****************************************
/**
 * @brief Writes one byte into the EEPROM.
 *
 * After writing, the current address will be increased by one.
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @param data
 * 8-bit data to be written.
 *
 * @sa eeprom_write_uint16(), eeprom_read(),
 *   eeprom_adress_set(), eeprom_adress_get()
 */
void eeprom_write(uint8_t data);

/**
 * @brief Reads one byte from the EEPROM.
 *
 * After reading, the current address will be increased by one.
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @result
 * 8-bit data beeing read.
 *
 * @sa eeprom_write(), eeprom_read_uint16(),
 *   eeprom_adress_set(), eeprom_adress_get()
 */
uint8_t eeprom_read(void);

/**
 * @brief Writes one word into the EEPROM.
 *
 * After writing, the current address will be increased by two.
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @param data
 * 16-bit data to be written.
 *
 * @sa eeprom_write(), eeprom_read_uint16(),
 *   eeprom_adress_set(), eeprom_adress_get()
 */
void eeprom_write_uint16(uint16_t data);

/**
 * @brief Reads one word from the EEPROM.
 *
 * After reading, the current address will be increased by two.
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @result
 * 16-bit data beeing read.
 *
 * @sa eeprom_write_uint16(), eeprom_read(),
 *   eeprom_adress_set(), eeprom_adress_get()
 */
uint16_t eeprom_read_uint16(void);

/**
 * @brief Sets the current adress of the EEPROM.
 *
 * @param data
 * 16-bit data to be written.
 *
 * @sa eeprom_adress_get(), eeprom_write(), eeprom_read()
 */
void eeprom_adress_set(uint16_t adr);

/**
 * @brief Returns the current adress of the EEPROM.
 *
 * @result
 * 16-bit data beeing read.
 *
 * @sa eeprom_adress_set(), eeprom_write(), eeprom_read()
 */
uint16_t eeprom_adress_get(void);

/**
 * @brief Prints general information about the eeprom.
 *
 * This function prints all the data related to the eeprom modul.
 *
 * The output is similar to the following: \n
 * @code
 * eeprom:
 *   adress      : 0x0145
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
 * @sa robolib_eeprom_print_compiled()
 */

#ifndef __DOXYGEN__
    void robolib_eeprom_print(void (*out)(uint8_t));
#else
    void robolib_eeprom_print(void* out);
#endif

/**
 * @brief Prints information about the version of the eeprom.
 *
 * This function prints information about the source file(s)
 * of this modul at compile time. This inlcude the version of
 * the main files and important macros.
 *
 * The output is similar to the following: \n
 * @code
 * /source/eeprom.c 15.07.2015 V0.0.0
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
 * @sa robolib_eeprom_print()
 */
#ifndef __DOXYGEN__
    void robolib_eeprom_print_compiled(void (*out)(uint8_t));
#else
    void robolib_eeprom_print_compiled(void* out);
#endif


/**
 * @brief Internal function.
 *
 * Do not call this function at all.
 */
void robolib_eeprom_init(void);

// automated initializing
#ifndef __DOXYGEN__
    __inline__ void robolib_eeprom_init_inline(void) {
        #ifdef ROBOLIB_INIT
            ROBOLIB_INIT();
        #endif

        robolib_eeprom_init();
    }

    #undef ROBOLIB_INIT
    #define ROBOLIB_INIT()     robolib_eeprom_init_inline()
    #define robolib_init()     cli(); ROBOLIB_INIT(); sei()
    #define robolib_init_cli() cli(); ROBOLIB_INIT()
#endif

#endif // #ifndef _ROBOLIB_EEPROM_H_
