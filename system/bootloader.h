/******************************************************************************
* system/bootloader.h                                                         *
* ===================                                                         *
*                                                                             *
* Version: 1.0.2                                                              *
* Date   : 26.10.15                                                           *
* Author : Peter Weissig                                                      *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/peterweissig/robolib                                   *
******************************************************************************/

#ifndef _SYSTEM_BOOTLOADER_H_
#define _SYSTEM_BOOTLOADER_H_

//**************************<Included files>***********************************
#include <inttypes.h>
#include <avr/boot.h>
#include <avr/interrupt.h>

//**************************<Macros>*******************************************
#ifndef __DOXYGEN__

    // warnings and errors
    #ifndef BOOTSTART
        #error "BOOTSTART must be defined"
    #endif // #ifndef (BOOTSTART)

    #if (BOOTSTART < 0) || (BOOTSTART >= FLASHEND)
        #error "BOOTSTART must be between 0 and FLASHEND"
    #endif // #if (BOOTSTART < 0) || (BOOTSTART >= FLASHEND)


    // for automated initializing see end of this file

#endif // #ifndef doxygen

//! The number of bytes the bootloader modul uses in order to work.
//! This number is architecture dependend
#if (FLASHEND > 0xFFFF)
    #define SYSTEM_RAM_COUNT_BOOTLOADER 4
#else //#if (FLASHEND > 0xFFFF)
    #define SYSTEM_RAM_COUNT_BOOTLOADER 2
#endif //#if (FLASHEND > 0xFFFF)

//**************************<Types>********************************************

//**************************<Functions>****************************************
/**
 * @brief User function, called to decide if bootloader should be started.
 *
 * This function is called upon (re)start of MCU, it must check if
 * the bootloader should be started or normal operation is desired.
 *
 * This function must be implemented by the user!
 *
 * @return
 * Boolean value. \n
 * False (== 0x00) means normal operation.                  \n
 * True  (!= 0x00) means bootloader is started.             \n
 *
 * @sa bootloader_end()
 */
uint8_t bootloader_start(void);

/**
 * @brief User function, called when bootloader is terminated.
 *
 * This function is called upon termination of bootloader.
 * It is intented to reconfigure hardware
 * before starting normal operation.
 *
 * This function must be implemented by the user!
 *
 * @sa bootloader_start()
 */
void bootloader_end(void);

/**
 * @brief User function, called when the bootloader wants to read a new byte.
 *
 * This function is called every time the bootloader trys to receive
 * a new byte - either for commands or as data.
 * This function has to return the data in the right order - there must be
 * a fifo buffer (first in first out).
 *
 * If no data is available the function must wait.
 *
 * This function must be implemented by the user!
 *
 * @return
 * One byte from communication channel
 *
 * @sa bootloader_data_stat(), bootloader_data_out()
 */
uint8_t bootloader_data_in(void);

/**
 * @brief User function, called when the bootloader wants to check for new data.
 *
 * This function is called every time the bootloader trys to check if data
 * was received.
 *
 * This function must be implemented by the user!
 *
 * @return
 * Boolean value. \n
 * False (== 0x00) means no data.                           \n
 * True  (!= 0x00) means new data is available in buffer.   \n
 *
 * @sa bootloader_data_in(), bootloader_data_out()
 */
uint8_t bootloader_data_stat(void);

/**
 * @brief User function, called when the bootloader wants to write one byte.
 *
 * This function is called every time the bootloader trys to send
 * a new byte - either for commands or as data.
 *
 * This function must be implemented by the user!
 *
 * @param data
 * one byte of data to be send
 *
 * @sa bootloader_data_in(), bootloader_data_stat()
 */
void bootloader_data_out(uint8_t data);

/**
 * @brief User function, called when no command is executed.
 *
 * This function is called every 50 milliseconds if no command is executed.
 *
 * This function must be implemented by the user!
 *
 * @return
 * Boolean value. \n
 * False (== 0x00) means bootloader will be terminated.     \n
 * True  (!= 0x00) means bootloader keeps running.          \n
 *
 * @sa bootloader_command(), bootloader_end()
 */
uint8_t bootloader_idle(void);

/**
 * @brief User function, called when any command is executed.
 *
 * This function is called for every command which is executed.
 * Also for the help-command.
 *
 * This function must be implemented by the user!
 *
 * @sa bootloader_help(), bootloader_wait()
 */
void bootloader_command(void);

/**
 * @brief User function, called when help-command is executed.
 *
 * This function is called upon execution of help command.
 * It is intented to print useful information.
 *
 * This function must be implemented by the user!
 *
 * @sa bootloader_error()
 */
void bootloader_help(void);

/**
 * @brief User function, called when an unknown command is executed.
 *
 * This function is called upon reception of an unknown command.
 * This can be caused by communication error.
 *
 * This function must be implemented by the user!
 *
 * @return
 * Boolean value. \n
 * False (== 0x00) means bootloader will be terminated.     \n
 * True  (!= 0x00) means bootloader keeps running.          \n
 *
 * @sa bootloader_help(), bootloader_end()
 */
uint8_t bootloader_error(void);

/**
 * @brief Prints general information about the bootloader.
 *
 * This function prints all the data related to the bootloader modul.
 *
 * The output is similar to the following: \n
 * @code
 * bootloader:
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
    void system_bootloader_print(void (*out)(uint8_t));
#else
    void system_bootloader_print(void* out);
#endif

/**
 * @brief Prints information about the version of the bootloader.
 *
 * This function prints information about the source file(s)
 * of this modul at compile time. This includes the version of
 * the main files and important macros.
 *
 * The output is similar to the following: \n
 * @code
 * /source/bootloader.c 23.09.2015 V0.0.0
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
 * @sa system_bootloader_print()
 */
#ifndef __DOXYGEN__
    void system_bootloader_print_compiled(void (*out)(uint8_t));
#else
    void system_bootloader_print_compiled(void* out);
#endif


/**
 * @brief Internal function.
 *
 * Do not call this function at all.
 */
void system_bootloader_init(void);

// automated initializing
#ifndef __DOXYGEN__
    __inline__ void system_bootloader_init_inline(void) {
        #ifdef SYSTEM_INIT
            SYSTEM_INIT();
        #endif;

        system_bootloader_init();
    }

    #define SYSTEM_INIT()     system_bootloader_init_inline()
    #define system_init()     cli(); SYSTEM_INIT(); sei()
    #define system_init_cli() cli(); SYSTEM_INIT()
#endif

#endif // #ifndef _SYSTEM_BOOTLOADER_H_
