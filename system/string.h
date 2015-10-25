/******************************************************************************
* system/string.h                                                             *
* ===============                                                             *
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

#ifndef _SYSTEM_STRING_H_
#define _SYSTEM_STRING_H_


//**************************<Macros>*******************************************
//! The number of bytes the string modul uses in order to work
#define SYSTEM_RAM_COUNT_STRING 0

#ifndef __DOXYGEN__

  // for automated initializing see end of this file

#endif // #ifndef doxygen

//**************************<Included files>***********************************
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

//**************************<Types>********************************************

//**************************<Functions>****************************************
/**
 * @brief Converts an unsigned 16-bit integer to a string.
 *
 * This function takes an <b> unsigned </b> 16-bit integer
 * and converts it to a string.
 *
 * The parameter digits can be used to right justify the result.
 * If digits is higher then the actual number of used characters
 * then the final string will be padded with spaces.
 *
 * The resulting string is passed byte by byte to the given
 * output function (e.g. uart0_send());
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @param out
 * Output function of type "void out(uint8_t)"
 *
 * @param number
 * Value which is converted.
 *
 * @param digits
 * Minimum number of characters used for the string. [0..32]
 *
 * @sa string_from_int(), string_from_hex(), string_from_bits(),
 *     string_from_bool()
 */
#ifndef __DOXYGEN__
    void string_from_uint(void (*out)(uint8_t), uint16_t number,
      uint8_t digits);
#else
    void string_from_uint(void  *out          , uint16_t number,
      uint8_t digits);
#endif

/**
 * @brief Converts a signed 16-bit integer to a string.
 *
 * This function takes a <b> signed </b> 16-bit integer
 * and converts it to a string.
 *
 * The parameter digits can be used to right justify the result.
 * If digits is higher then the actual number of used characters
 * then the final string will be padded with spaces.
 *
 * The resulting string is passed byte by byte to the given
 * output function (e.g. uart0_send());
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @param out
 * Output function of type "void out(uint8_t)"
 *
 * @param number
 * Value which is converted.
 *
 * @param digits
 * Minimum number of characters used for the string. [0..32]
 *
 * @sa string_from_uint(), string_from_hex(), string_from_bits(),
 *     string_from_bool()
 */
#ifndef __DOXYGEN__
    void string_from_int(void (*out)(uint8_t), int16_t number, uint8_t digits);
#else
    void string_from_int(void  *out          , int16_t number, uint8_t digits);
#endif

/**
 * @brief Converts a hexadecimal number to a string.
 *
 * This function takes a hexadecimal number and converts it to a string.
 *
 * The parameter digits sets the number of hexadecimal digits.
 * If digits is less than 0 then a "0x" will be added
 * before the hexadecimal digits.
 *
 * The resulting string is passed byte by byte to the given
 * output function (e.g. uart0_send());
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @param out
 * Output function of type "void out(uint8_t)"
 *
 * @param number
 * Value which is converted.
 *
 * @param digits
 * Number of used hexadecimal digits. [-4..-1,1..4]
 *
 * @sa string_from_int(), string_from_uint(), string_from_bits(),
 *     string_from_bool()
 */
#ifndef __DOXYGEN__
    void string_from_hex(void (*out)(uint8_t), uint16_t number, int8_t digits);
#else
    void string_from_hex(void  *out          , uint16_t number, int8_t digits);
#endif

/**
 * @brief Converts a bitmask to a string.
 *
 * This function takes a bitmask and converts it to a string.
 *
 * The parameter digits sets the number of bits.
 * If digits is less than 0 then a "0b" will be added
 * before the bits.
 *
 * The resulting string is passed byte by byte to the given
 * output function (e.g. uart0_send());
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @param out
 * Output function of type "void out(uint8_t)"
 *
 * @param bitmask
 * Value which is converted.
 *
 * @param digits
 * Number of used bits. [-16..-1,1..16]
 *
 * @sa string_from_int(), string_from_uint(), string_from_hex(),
 *     string_from_bool()
 */
#ifndef __DOXYGEN__
    void string_from_bits(void (*out)(uint8_t), uint16_t bitmask,
      int8_t digits);
#else
    void string_from_bits(void  *out          , uint16_t bitmask,
      int8_t digits);
#endif

/**
 * @brief Converts a boolean to a string.
 *
 * This function takes a boolean and converts it to a string.
 *
 * If the boolean value is true  (!= 0) the string "true" will be returned.
 * Otherwise the string "false" will be returned.
 *
 * The resulting string is passed byte by byte to the given
 * output function (e.g. uart0_send());
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @param out
 * Output function of type "void out(uint8_t)"
 *
 * @param boolean
 * Value which is converted.
 *
 * @sa string_from_int(), string_from_uint(), string_from_hex(),
 *     string_from_bits()
 */
#ifndef __DOXYGEN__
    void string_from_bool(void (*out)(uint8_t), uint8_t boolean);
#else
    void string_from_bool(void  *out          , uint8_t boolean);
#endif

/**
 * @brief Sends a string located in the RAM.
 *
 * This function loads the referenced string character by character
 * and passes those to an output function until a termating
 * \\ 0-character is found.
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @param out
 * Output function of type "void out(uint8_t)"
 *
 * @param str
 * Pointer to the start of the string located in the RAM.
 *
 * @sa string_from_const(), string_from_flash()
 */
#ifndef __DOXYGEN__
    void string_from_ram(void (*out)(uint8_t), const char *str);
#else
    void string_from_ram(void  *out          , const char *str);
#endif

/**
 * @brief Sends a string located in the flash.
 *
 * This function loads the referenced string character by character
 * and passes those to an output function until a termating
 * \\ 0-character is found.
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @param out
 * Output function of type "void out(uint8_t)"
 *
 * @param str
 * Pointer to the start of the string located in the flash.
 *
 * @sa string_from_const(), string_from_ram()
 */

#ifndef __DOXYGEN__
    void string_from_flash(void (*out)(uint8_t), const char *str);
#else
    void string_from_flash(void  *out          , const char *str);
#endif

/**
 * @brief Sends a string with fixed length located in the flash.
 *
 * This function loads the referenced string character by character
 * and passes those to an output function until a termating
 * \\ 0-character is found.
 *
 * If the string is longer then specified by length it will be truncated.
 * Otherwise it will be padded with spaces while being left justified.
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @param out
 * Output function of type "void out(uint8_t)"
 *
 * @param str
 * Pointer to the start of the string located in the flash.
 *
 * @param length
 * Length of resulting string [1..255]
 *
 *
 * @sa string_from_flash(), string_from_const_length()
 */
#ifndef __DOXYGEN__
    void string_from_flash_length(void (*out)(uint8_t), const char *str,
      uint8_t length);
#else
    void string_from_flash_length(void  *out          , const char *str,
      uint8_t length);
#endif

/**
 * @brief Sends a string constant.
 *
 * This function puts the giving string constant in flash during compile time
 * and sends it with the string_from_flash function.
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @param out
 * Output function of type "void out(uint8_t)"
 *
 * @param str
 * String constant
 *
 * @sa string_from_flash(), string_from_ram()
 */
#ifndef __DOXYGEN__
    #define string_from_const(out, str) string_from_flash(out, PSTR(str))
#else
    void string_from_const(void *out, const char *str);
#endif

/**
 * @brief Sends a string constant with fixed length.
 *
 * This function puts the giving string constant in flash during compile time
 * and sends it with the string_from_flash function.
 *
 * If the string is longer then specified by length it will be truncated.
 * Otherwise it will be padded with spaces while being left justified.
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @param out
 * Output function of type "void out(uint8_t)"
 *
 * @param str
 * String constant
 *
 * @param length
 * Length of resulting string [1..255]
 *
 * @sa string_from_const(), string_from_flash_length()
 */
#ifndef __DOXYGEN__
    #define string_from_const_length(out, str, len) \
      string_from_flash_length(out, PSTR(str), len)
#else
    void string_from_const_length(void *out, const char *str, uint8_t length);
#endif

/**
 * @brief Sends a string derived from a macro.
 *
 * This function wraps the content of the given macro in a string
 * (stringify it), puts it in flash during compile time
 * and sends it with the string_from_flash function.
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @param out
 * Output function of type "void out(uint8_t)"
 *
 * @param macro
 * Macro which content is send
 *
 * @sa string_from_const(), string_from_flash()
 */
#ifndef __DOXYGEN__
  #define STRING_FROM_MACRO(out, str) string_from_flash(out, PSTR(#str))
  #define string_from_macro(out, str) STRING_FROM_MACRO(out, str)
#else
  void string_from_macro(void* out, MACRO macro);
#endif

/**
 * @brief Prints general information about the string.
 *
 * This function prints all the data related to the string modul.
 *
 * The output is similar to the following: \n
 * @code
 * string:
 *   [empty]
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
 * @sa system_string_print_compiled()
 */
#ifndef __DOXYGEN__
    void system_string_print(void (*out)(uint8_t));
#else
    void system_string_print(void* out);
#endif

/**
 * @brief Prints information about the version of the string.
 *
 * This function prints information about the source file(s)
 * of this modul at compile time. This inlcude the version of
 * the main files and important macros.
 *
 * The output is similar to the following: \n
 * @code
 * /source/string.c 15.09.2014 V0.0.0
 *   [empty]
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
 * @sa system_string_print()
 */
#ifndef __DOXYGEN__
    void system_string_print_compiled(void (*out)(uint8_t));
#else
    void system_string_print_compiled(void* out);
#endif


// automated initializing
#ifndef __DOXYGEN__
    __inline__ void system_string_init_inline(void) {
        #ifdef SYSTEM_INIT
            SYSTEM_INIT();
        #endif;
    }

    #define SYSTEM_INIT()     system_string_init_inline()
    #define system_init()     cli(); SYSTEM_INIT(); sei()
    #define system_init_cli() cli(); SYSTEM_INIT()
#endif

#endif // #ifndef _SYSTEM_STRING_H_
