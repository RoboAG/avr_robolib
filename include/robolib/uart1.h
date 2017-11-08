/******************************************************************************
* robolib/uart1.h                                                             *
* ===============                                                             *
*                                                                             *
* Version: 1.0.1                                                              *
* Date   : 26.10.15                                                           *
* Author : Peter Weissig                                                      *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/peterweissig/robolib                                   *
******************************************************************************/

#ifndef _ROBOLIB_UART1_H_
#define _ROBOLIB_UART1_H_


//**************************<Macros>*******************************************
#ifndef __DOXYGEN__
    #ifndef UART1_BAUD
        #define UART1_BAUD 57600
    #endif // if !defined(UART1_BAUD)

    // warnings and errors
    #ifdef UART1_TX
        #if UART1_TX > 3
            #if UART1_TX > 256
                #warning "transmit buffer for UART1 is to huge (> 256)"
                #define UART1_TX 256
            #endif // if UART1_TX > 256
        #else // if UART1_TX > 3
            #warning "transmit buffer for UART1 is to small (< 3)"
            #undef UART1_TX
        #endif // if UART1_TX > 3
    #endif // ifdef UART1_TX

    #ifdef UART1_RX
        #if UART1_RX > 3
            #if UART1_RX > 256
                #warning "receive buffer for UART1 is to huge (> 256)"
                #define UART1_RX 256
            #endif // if UART1_RX > 256
        #else // if UART1_RX > 3
            #warning "receive buffer for UART1 is to small (< 3)"
            #undef UART1_RX
        #endif // if UART1_RX > 3
    #endif // ifdef UART1_RX

    #ifndef F_CPU
        #error "F_CPU must be set (e.g. 16MHz: 16000000)"
    #endif // #ifndef F_CPU

    // for automated initializing see end of this file

#endif // #ifndef doxygen

//! The number of bytes the uart1 modul uses in order to work
#ifdef UART1_RX
    #ifdef UART1_TX
        #define ROBOLIB_RAM_COUNT_UART1 UART1_RX + 2 + UART1_TX + 2
    #else //#ifdef UART1_TX
        #define ROBOLIB_RAM_COUNT_UART1 UART1_RX + 2
    #endif //#ifdef UART1_TX
#else //#ifdef UART1_RX
    #ifdef UART1_TX
        #define ROBOLIB_RAM_COUNT_UART1 UART1_TX + 2
    #else //#ifdef UART1_TX
        #define ROBOLIB_RAM_COUNT_UART1 0
    #endif //#ifdef UART1_TX
#endif //#ifdef UART1_RX

//**************************<Included files>***********************************
#include <inttypes.h>
#include <avr/interrupt.h>

//**************************<Types>********************************************

//**************************<Functions>****************************************
/**
 * @brief Transmits one byte to the UART1.
 *
 * This function transmits one byte of data over the UART1.
 *
 * If the transmit buffer is used the data will be stored within
 * that buffer and send at the time the UART1 is available again.
 * Otherwise only the hardware buffer is used.
 *
 * In each case this function will <b> wait </b> if the buffer is full.
 * If the UART1 is disabled while waiting the function will fail and return.
 *
 * The transmit buffer will be used if the preprocessor switch
 * UART1_TX is set between 3 and 256.
 *
 * This function can be called from within an interrupt,
 * but it will enable the global interrupt flag.
 *
 * @param data
 * 8-bit data to be send.
 *
 * @sa uart1_send_nonblocking(), uart1_get(), uart1_txcount_get()
 */
void uart1_send(uint8_t data);

/**
 * @brief Trys to transmits one byte to the UART1.
 *
 * This function transmits one byte of data over UART1,
 * if it does not have to wait.
 *
 * If the transmit buffer is used the data will be stored within
 * that buffer and send at the time the UART1 is available again.
 * Otherwise only the hardware buffer is used.
 *
 * In each case this function will fail if the buffer is full.
 *
 * The transmit buffer will be used if the preprocessor switch
 * UART1_TX is set between 3 and 256.
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
 * @sa uart1_send(), uart1_get_nonblocking(), uart1_txcount_get()
 */
uint8_t uart1_send_nonblocking(uint8_t data);

/**
 * @brief Recieves one byte from the UART1.
 *
 * This function receives one byte of data from UART1.
 *
 * If the recieve buffer is used and contains unread data then the
 * first unread byte is returned. Otherwise the function will check
 * the hardware buffer in the same way.
 *
 * In each case this function will <b> wait </b> if the buffer is empty.
 * If the UART1 is disabled while waiting the function will fail and return 0x00.
 *
 * The recieve buffer will be used if the preprocessor switch
 * UART1_RX is set between 3 and 256.
 *
 * This function can be called from within an interrupt,
 * but it will enable the global interrupt flag.
 *
 * @return
 * 8-bit data which was recieved first.
 *
 * @sa uart1_get_nonblocking(), uart1_send(), uart1_rxcount_get()
 */
uint8_t uart1_get(void);

/**
 * @brief Trys to recieves one byte from the UART1.
 *
 * This function receives one byte of data from UART1,
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
 * UART1_RX is set between 3 and 256.
 *
 * This function can be called from within an interrupt,
 * but it will enable the global interrupt flag.
 *
 * @return
 * 8-bit data which was recieved first.
 *
 * @sa uart1_get(), uart1_send_nonblocking(), uart1_rxcount_get()
 */
uint8_t uart1_get_nonblocking(void);

/**
 * @brief Returns the number of bytes to be transmitted to UART1.
 *
 * This function is only working if the transmit buffer is used.
 * Otherwise it does not return the number of bytes, but a boolean
 * information if new data can be send.
 *
 * The transmit buffer will be used if the preprocessor switch
 * UART1_TX is set between 3 and 256.
 *
 * This function can be called from within an interrupt.
 *
 * @return
 * Number of bytes in the buffer to be sent.
 *
 * @sa uart1_send(), uart1_rxcount_get(), uart1_flush()
 */
uint8_t uart1_txcount_get(void);

/**
 * @brief Returns the number of unread bytes of UART1.
 *
 * This function is only wokring if the recieve buffer is used.
 * Otherwise it does not return the number of bytes, but a boolean
 * information if new data is available.
 *
 * The recieve buffer will be used if the preprocessor switch
 * UART1_RX is set between 3 and 256.
 *
 * This function can be called from within an interrupt.
 *
 * @return
 * Number of unread bytes within the recieve buffer.
 *
 * @sa uart1_get(), uart1_txcount_get(), uart1_flush()
 */
uint8_t uart1_rxcount_get(void);

/**
 * @brief Clears the receive and transmit buffer of UART1.
 *
 * This function clears the receive and transmit buffer of the UART1.
 *
 * The transmit buffer will be used if the preprocessor switch
 * UART1_TX is set between 3 and 256.
 *
 * The recieve buffer will be used if the preprocessor switch
 * UART1_RX is set between 3 and 256.
 *
 * This function can be called from within an interrupt.
 *
 * @sa uart1_send(), uart1_get(), uart1_txcount_get(), uart1_rxcount_get()
 */
void uart1_flush(void);

/**
 * @brief Sets the baudrate of UART1.
 *
 * This function sets the baudrate of UART1.
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
 * @sa uart1_baud_get(), uart1_send(), uart1_get()
 */
uint8_t uart1_baud_set(uint16_t baudrate);

/**
 * @brief Returns the baudrate of UART1.
 *
 * This function returns the baudrate of UART1.
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
 * Baudrate of UART1.
 *
 * @sa uart1_baud_set(), uart1_send(), uart1_get()
 */
uint16_t uart1_baud_get(void);

/**
 * @brief Enables the uart1 modul.
 *
 * This function enables the whole uart1 modul.
 *
 * This function can be called from within an interrupt.
 *
 * @sa uart1_is_enabled(), uart1_disable()
 */
void uart1_enable(void);

/**
 * @brief Disables the uart1 modul.
 *
 * This function disables the whole uart1 modul.
 *
 * This function can be called from within an interrupt.
 *
 * @sa uart1_is_enabled(), uart1_enable()
 */
void uart1_disable(void);

/**
 * @brief Returns if the uart1 modul is enabled.
 *
 * This function returns the state of the uart1 modul.
 *
 * This function can be called from within an interrupt.
 *
 * @return
 * Boolean value. \n
 * False (== 0x00) means the uart1 is disabled.        \n
 * True  (!= 0x00) means the uart1 is enabled.         \n
 *
 * @sa uart1_enable(), uart1_disable()
 */
uint8_t uart1_is_enabled(void);

/**
 * @brief Prints general information about the uart1.
 *
 * This function prints all the data related to the uart1 modul.
 *
 * The output is similar to the following: \n
 * @code
 * uart1:
 *   enabled     : true
 *   baud        : 9600
 *   tx          : 3
 *   rx          : 50
 *    ...
 * @endcode
 *
 * This function passes the resulting string byte by byte to the
 * output function (e.g. uart1_send()).
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @param out
 * Output function of type "void out(uint8_t)"
 *
 * @sa robolib_uart1_print_compiled()
 */
#ifndef __DOXYGEN__
    void robolib_uart1_print(void (*out)(uint8_t));
#else
    void robolib_uart1_print(void* out);
#endif

/**
 * @brief Prints information about the version of the uart1.
 *
 * This function prints information about the source file(s)
 * of this modul at compile time. This inlcude the version of
 * the main files and important macros.
 *
 * The output is similar to the following: \n
 * @code
 * /source/uart1.c 14.09.2014 V0.0.0
 * /source/uart1/uart1_atmega2561.c 14.09.2014 V0.0.0
 * MCU         : atmega2561
 * F_CPU       : 16000000
 * UART1_RX    : 64
 * UART1_TX    : 64
 *    ...
 * @endcode
 *
 * This function passes the resulting string byte by byte to the
 * output function (e.g. uart1_send()).
 *
 * This function must be handled with care when called from within an interrupt.
 *
 * @param out
 * Output function of type "void out(uint8_t)"
 *
 * @sa robolib_uart1_print()
 */
#ifndef __DOXYGEN__
    void robolib_uart1_print_compiled(void (*out)(uint8_t));
#else
    void robolib_uart1_print_compiled(void* out);
#endif


/**
 * @brief Internal function.
 *
 * Do not call this function at all.
 */
void robolib_uart1_init(void);

// automated initializing
#ifndef __DOXYGEN__
    __inline__ void robolib_uart1_init_inline(void) {
        #ifdef ROBOLIB_INIT
            ROBOLIB_INIT();
        #endif

        robolib_uart1_init();
    }

    #undef ROBOLIB_INIT
    #define ROBOLIB_INIT()     robolib_uart1_init_inline()
    #define robolib_init()     cli(); ROBOLIB_INIT(); sei()
    #define robolib_init_cli() cli(); ROBOLIB_INIT()
#endif

#endif // #ifndef _ROBOLIB_UART1_H_
