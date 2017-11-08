/******************************************************************************
* robolib/uart0.h                                                             *
* ===============                                                             *
*                                                                             *
* Version: 1.0.1                                                              *
* Date   : 26.10.15                                                           *
* Author : Peter Weissig                                                      *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/peterweissig/robolib                                   *
******************************************************************************/

#ifndef _ROBOLIB_UART0_H_
#define _ROBOLIB_UART0_H_


//**************************<Macros>*******************************************
#ifndef __DOXYGEN__
    #ifndef UART0_BAUD
        #define UART0_BAUD 57600
    #endif // if !defined(UART0_BAUD)

    // warnings and errors
    #ifdef UART0_TX
        #if UART0_TX > 3
            #if UART0_TX > 256
                #warning "transmit buffer for UART0 is to huge (> 256)"
                #define UART0_TX 256
            #endif // if UART0_TX > 256
        #else // if UART0_TX > 3
            #warning "transmit buffer for UART0 is to small (< 3)"
            #undef UART0_TX
        #endif // if UART0_TX > 3
    #endif // ifdef UART0_TX

    #ifdef UART0_RX
        #if UART0_RX > 3
            #if UART0_RX > 256
                #warning "receive buffer for UART0 is to huge (> 256)"
                #define UART0_RX 256
            #endif // if UART0_RX > 256
        #else // if UART0_RX > 3
            #warning "receive buffer for UART0 is to small (< 3)"
            #undef UART0_RX
        #endif // if UART0_RX > 3
    #endif // ifdef UART0_RX

    #ifndef F_CPU
        #error "F_CPU must be set (e.g. 16MHz: 16000000)"
    #endif // #ifndef F_CPU


    // for automated initializing see end of this file

#endif // #ifndef doxygen

//! The number of bytes the uart0 modul uses in order to work
#ifdef UART0_RX
    #ifdef UART0_TX
        #define ROBOLIB_RAM_COUNT_UART0 UART0_RX + 2 + UART0_TX + 2
    #else //#ifdef UART0_TX
        #define ROBOLIB_RAM_COUNT_UART0 UART0_RX + 2
    #endif //#ifdef UART0_TX
#else //#ifdef UART0_RX
    #ifdef UART0_TX
        #define ROBOLIB_RAM_COUNT_UART0 UART0_TX + 2
    #else //#ifdef UART0_TX
        #define ROBOLIB_RAM_COUNT_UART0 0
    #endif //#ifdef UART0_TX
#endif //#ifdef UART0_RX

//**************************<Included files>***********************************
#include <inttypes.h>
#include <avr/interrupt.h>

//**************************<Types>********************************************

//**************************<Functions>****************************************
/**
 * @brief Transmits one byte to the UART0.
 *
 * This function transmits one byte of data over the UART0.
 *
 * If the transmit buffer is used the data will be stored within
 * that buffer and send at the time the UART0 is available again.
 * Otherwise only the hardware buffer is used.
 *
 * In each case this function will <b> wait </b> if the buffer is full.
 * If the UART0 is disabled while waiting the function will fail and return.
 *
 * The transmit buffer will be used if the preprocessor switch
 * UART0_TX is set between 3 and 256.
 *
 * This function can be called from within an interrupt,
 * but it will enable the global interrupt flag.
 *
 * @param data
 * 8-bit data to be send.
 *
 * @sa uart0_send_nonblocking(), uart0_get(), uart0_txcount_get()
 */
void uart0_send(uint8_t data);

/**
 * @brief Trys to transmits one byte to the UART0.
 *
 * This function transmits one byte of data over UART0,
 * if it does not have to wait.
 *
 * If the transmit buffer is used the data will be stored within
 * that buffer and send at the time the UART0 is available again.
 * Otherwise only the hardware buffer is used.
 *
 * In each case this function will fail if the buffer is full.
 *
 * The transmit buffer will be used if the preprocessor switch
 * UART0_TX is set between 3 and 256.
 *
 * This function can be called from within an interrupt.
 *
 * @param data
 * 8-bit data to be send.
 *
 * @return
 * Boolean value. \n
 * False (== 0x00) means the function failed. \n
 * True  (!= 0x00) means the data is or will be transmitted. \n
 *
 * @sa uart0_send(), uart0_get_nonblocking(), uart0_txcount_get()
 */
uint8_t uart0_send_nonblocking(uint8_t data);

/**
 * @brief Recieves one byte from the UART0.
 *
 * This function receives one byte of data from UART0.
 *
 * If the recieve buffer is used and contains unread data then the
 * first unread byte is returned. Otherwise the function will check
 * the hardware buffer in the same way.
 *
 * In each case this function will <b> wait </b> if the buffer is empty.
 * If the UART0 is disabled while waiting the function will fail and return 0x00.
 *
 * The recieve buffer will be used if the preprocessor switch
 * UART0_RX is set between 3 and 256.
 *
 * This function can be called from within an interrupt,
 * but it will enable the global interrupt flag.
 *
 * @return
 * 8-bit data which was recieved first.
 *
 * @sa uart0_get_nonblocking(), uart0_send(), uart0_rxcount_get()
 */
uint8_t uart0_get(void);

/**
 * @brief Trys to recieves one byte from the UART0.
 *
 * This function receives one byte of data from UART0,
 * if it does not have to wait.
 *
 * If the recieve buffer is used and contains unread data then the
 * first unread byte is returned. Otherwise the function will check
 * the hardware buffer in the same way.
 *
 * In each case this function will fail if the buffer is empty
 * and returns 0x00;
 *
 * The recieve buffer will be used if the preprocessor switch
 * UART0_RX is set between 3 and 256.
 *
 * This function can be called from within an interrupt,
 * but it will enable the global interrupt flag.
 *
 * @return
 * 8-bit data which was recieved first.
 *
 * @sa uart0_get(), uart0_send_nonblocking(), uart0_rxcount_get()
 */
uint8_t uart0_get_nonblocking(void);

/**
 * @brief Returns the number of bytes to be transmitted to UART0.
 *
 * This function is only working if the transmit buffer is used.
 * Otherwise it does not return the number of bytes, but a boolean
 * information if new data can be send.
 *
 * The transmit buffer will be used if the preprocessor switch
 * UART0_TX is set between 3 and 256.
 *
 * This function can be called from within an interrupt.
 *
 * @return
 * Number of bytes in the buffer to be sent.
 *
 * @sa uart0_send(), uart0_rxcount_get(), uart0_flush()
 */
uint8_t uart0_txcount_get(void);

/**
 * @brief Returns the number of unread bytes of UART0.
 *
 * This function is only wokring if the recieve buffer is used.
 * Otherwise it does not return the number of bytes, but a boolean
 * information if new data is available.
 *
 * The recieve buffer will be used if the preprocessor switch
 * UART0_RX is set between 3 and 256.
 *
 * This function can be called from within an interrupt.
 *
 * @return
 * Number of unread bytes within the recieve buffer.
 *
 * @sa uart0_get(), uart0_txcount_get(), uart0_flush()
 */
uint8_t uart0_rxcount_get(void);

/**
 * @brief Clears the receive and transmit buffer of UART0.
 *
 * This function clears the receive and transmit buffer of the UART0.
 *
 * The transmit buffer will be used if the preprocessor switch
 * UART0_TX is set between 3 and 256.
 *
 * The recieve buffer will be used if the preprocessor switch
 * UART0_RX is set between 3 and 256.
 *
 * This function can be called from within an interrupt.
 *
 * @sa uart0_send(), uart0_get(), uart0_txcount_get(), uart0_rxcount_get()
 */
void uart0_flush(void);

/**
 * @brief Sets the baudrate of UART0.
 *
 * This function sets the baudrate of UART0.
 *
 * The value of the baudrate registers are calculated based
 * on the given baudrate and the cpu frequency.
 * If the result is not in the given range this function
 * fails and returns 0x00;
 *
 * Only integer arithmetics is used and therefore rounding errors need
 * to be considered.
 *
 * All current ongoing communication will be corrupted.
 *
 * This function can be called from within an interrupt.
 *
 * @param baudrate
 * 16-bit value representing the new baudrate
 *
 * @return
 * Boolean value. \n
 * False (== 0x00) means the baudrate was not udated.  \n
 * True  (!= 0x00) means the new baudrate was set.     \n
 *
 * @sa uart0_baud_get(), uart0_send(), uart0_get()
 */
uint8_t uart0_baud_set(uint16_t baudrate);

/**
 * @brief Returns the baudrate of UART0.
 *
 * This function returns the baudrate of UART0.
 *
 * The baudrate is calculated based on the baudrate registers and
 * the cpu frequency.
 *
 * Only integer arithmetics is used and therefore rounding errors need
 * to be considered.
 *
 * The result is only 16 bit wide.
 * If baudrate is higher than 65535 the result is set to this number.
 *
 * This function can be called from within an interrupt.
 *
 * @return
 * Baudrate of UART0.
 *
 * @sa uart0_baud_set(), uart0_send(), uart0_get()
 */
uint16_t uart0_baud_get(void);

/**
 * @brief Enables the uart0 modul.
 *
 * This function enables the whole uart0 modul.
 *
 * This function can be called from within an interrupt.
 *
 * @sa uart0_is_enabled(), uart0_disable()
 */
void uart0_enable(void);

/**
 * @brief Disables the uart0 modul.
 *
 * This function disables the whole uart0 modul.
 *
 * This function can be called from within an interrupt.
 *
 * @sa uart0_is_enabled(), uart0_enable()
 */
void uart0_disable(void);

/**
 * @brief Returns if the uart0 modul is enabled.
 *
 * This function returns the state of the uart0 modul.
 *
 * This function can be called from within an interrupt.
 *
 * @return
 * Boolean value. \n
 * False (== 0x00) means the uart0 is disabled.        \n
 * True  (!= 0x00) means the uart0 is enabled.         \n
 *
 * @sa uart0_enable(), uart0_disable()
 */
uint8_t uart0_is_enabled(void);

/**
 * @brief Prints general information about the uart0.
 *
 * This function prints all the data related to the uart0 modul.
 *
 * The output is similar to the following: \n
 * @code
 * uart0:
 *   enabled     : true
 *   baud        : 9600
 *   tx          : 3
 *   rx          : 50
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
 * @sa robolib_uart0_print_compiled()
 */
#ifndef __DOXYGEN__
    void robolib_uart0_print(void (*out)(uint8_t));
#else
    void robolib_uart0_print(void* out);
#endif

/**
 * @brief Prints information about the version of the uart0.
 *
 * This function prints information about the source file(s)
 * of this modul at compile time. This inlcude the version of
 * the main files and important macros.
 *
 * The output is similar to the following: \n
 * @code
 * /source/uart0.c 14.09.2014 V0.0.0
 * /source/uart0/uart0_atmega2561.c 14.09.2014 V0.0.0
 * MCU         : atmega2561
 * F_CPU       : 16000000
 * UART0_RX    : 64
 * UART0_TX    : 64
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
 * @sa robolib_uart0_print()
 */
#ifndef __DOXYGEN__
    void robolib_uart0_print_compiled(void (*out)(uint8_t));
#else
    void robolib_uart0_print_compiled(void* out);
#endif


/**
 * @brief Internal function.
 *
 * Do not call this function at all.
 */
void robolib_uart0_init(void);

// automated initializing
#ifndef __DOXYGEN__
    __inline__ void robolib_uart0_init_inline(void) {
        #ifdef ROBOLIB_INIT
            ROBOLIB_INIT();
        #endif

        robolib_uart0_init();
    }

    #undef ROBOLIB_INIT
    #define ROBOLIB_INIT()     robolib_uart0_init_inline()
    #define robolib_init()     cli(); ROBOLIB_INIT(); sei()
    #define robolib_init_cli() cli(); ROBOLIB_INIT()
#endif

#endif // #ifndef _ROBOLIB_UART0_H_
