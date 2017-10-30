/******************************************************************************
* source/uart0/uart0.c                                                        *
* ====================                                                        *
*                                                                             *
* Author : Peter Weissig                                                      *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/peterweissig/robolib                                   *
******************************************************************************/

//**************************<File version>*************************************
#define SYSTEM_UART0_VERSION \
    "source/uart0/uart0.c 29.10.2017 V1.0.3"

//**************************<Included files>***********************************
#include "system/uart0.h"

#include "system/assembler.h"
#include "system/string.h"
#include "system/types.h"

#include <avr/io.h>

//**************************<Variables>****************************************
#ifdef UART0_TX
    volatile uint8_t system_uart0_tx[UART0_TX];
    volatile uint8_t system_uart0_tx_start;
    volatile uint8_t system_uart0_tx_end;
#endif //#ifdef UART0_TX

#ifdef UART0_RX
    volatile uint8_t system_uart0_rx[UART0_RX];
    volatile uint8_t system_uart0_rx_start;
    volatile uint8_t system_uart0_rx_end;
#endif //#ifdef UART0_RX

//**************************<Prototypes>***************************************

//**************************<Renaming>*****************************************
#ifndef __DOXYGEN__
    #if !defined(USART0_UDRE_vect)
        #define USART0_UDRE_vect USART_UDRE_vect
    #endif // if !defined(USART0_UDRE_vect)
    #if !defined(USART0_RX_vect)
        #define USART0_RX_vect USART_RX_vect
    #endif // if !defined(USART0_UDRE_vect)
    #if !defined(UCSR0A)
        #define UCSR0A UCSRA
    #endif // if !defined(UCSR0A)
    #if !defined(UCSR0B)
        #define UCSR0B UCSRB
    #endif // if !defined(UCSR0B)
    #if !defined(UCSR0C)
        #define UCSR0C UCSRC
    #endif // if !defined(UCSR0C)
    #if !defined(UBRR0L)
        #define UBRR0L UBRRL
    #endif // if !defined(UBRR0L)
    #if !defined(UBRR0H)
        #define UBRR0H UBRRH
    #endif // if !defined(UBRR0H)
    #if !defined(UDR0)
        #define UDR0 UDR
    #endif // if !defined(UDR0)
    #if !defined(TXEN0)
        #define TXEN0 TXEN
    #endif // if !defined(TXEN0)
    #if !defined(RXEN0)
        #define RXEN0 RXEN
    #endif // if !defined(RXEN0)
    #if !defined(RXCIE0)
        #define RXCIE0 RXCIE
    #endif // if !defined(RXCIE0)
    #if !defined(UDRIE0)
        #define UDRIE0 UDRIE
    #endif // if !defined(UDRIE0)
    #if !defined(UDRE0)
        #define UDRE0 UDRE
    #endif // if !defined(UDRE0)
    #if !defined(TXC0)
        #define TXC0 TXC
    #endif // if !defined(TXC0)
    #if !defined(RXC0)
        #define RXC0 RXC
    #endif // if !defined(RXC0)
    #if !defined(USBS0)
        #define USBS0 USBS
    #endif // if !defined(USBS0)
    #if !defined(UCSZ00)
        #define UCSZ00 UCSZ0
    #endif // if !defined(UCSZ00)
    #if !defined(UCSZ01)
        #define UCSZ01 UCSZ1
    #endif // if !defined(UCSZ01)
#endif // #ifndef __DOXYGEN__

//**************************<Files>********************************************

#if   defined (__AVR_ATmega2561__) // switch micro controller
    #include <source/uart0/uart0_atmega2561.c>
    #define MCU atmega2561
#elif defined (__AVR_ATmega64__  ) // switch micro controller
    #include <source/uart0/uart0_atmega64.c>
    #define MCU atmega64
#elif defined (__AVR_ATmega644P__  ) // switch micro controller
    #include <source/uart0/uart0_atmega644p.c>
    #define MCU atmega644p
#elif defined (__AVR_ATmega328P__  ) // switch micro controller
    #include <source/uart0/uart0_atmega328p.c>
    #define MCU atmega328p
#else                              // switch micro controller
    #error "device is not supported (yet)"
#endif                             // switch micro controller

//**************************[uart0_send]*************************************** 27.09.2015
void uart0_send(uint8_t data) {

    #ifndef UART0_TX
        if ((UCSR0B & _BV(TXEN0)) == 0x00) {
            sei();
            return;
        }
    #endif // #ifndef UART0_TX

    while (uart0_send_nonblocking(data) == 0x00) {
        sei();
        if ((UCSR0B & _BV(TXEN0)) == 0x00) {
            return;
        }
    }
}

//**************************[uart0_send_nonblocking]*************************** 27.09.2015
#ifdef UART0_TX
    uint8_t uart0_send_nonblocking(uint8_t data) {

        uint8_t mSREG = SREG;

        cli();
        uint8_t temp_start = system_uart0_tx_start;
        uint8_t temp_end   = system_uart0_tx_end  ;

        // check if buffer is empty
        if (temp_start == temp_end) {
            if (UCSR0B & _BV(TXEN0)) {
                if (UCSR0A & _BV(UDRE0)) {
                    UDR0 = data;

                    SREG = mSREG;
                    return 0xFF;
                }
            }
        }

        // save data in buffer
        system_uart0_tx[temp_end] = data;

        // calculate next position
        temp_end++;
        if (temp_end >= UART0_TX) {
            temp_end-= UART0_TX;
        }

        // check if buffer is full
        if (temp_start == temp_end) {
            SREG = mSREG;
            return 0x00;
        }

        // save new buffer size
        system_uart0_tx_end = temp_end;

        // activate interrupt for transmission
        if (UCSR0B & _BV(TXEN0)) {
            UCSR0B|= _BV(UDRIE0);
        }

        SREG = mSREG;
        return 0xFF;
    }
#else //#ifdef UART0_TX
    uint8_t uart0_send_nonblocking(uint8_t data) {

        uint8_t mSREG = SREG;

        cli();

        if ((UCSR0B & _BV(TXEN0)) == 0x00) {
            SREG = mSREG;
            return 0x00;
        }

        if (UCSR0A & _BV(UDRE0)) {
            UDR0 = data;

            SREG = mSREG;
            return 0xFF;
        }

        SREG = mSREG;
        return 0x00;
    }
#endif //#ifdef UART0_TX

//**************************[uart0_get]**************************************** 29.10.2017
#ifdef UART0_RX
    uint8_t uart0_get() {

        uint8_t result;

        cli();
        uint8_t temp_start = system_uart0_rx_start;
        uint8_t temp_end   = system_uart0_rx_end  ;

        // check if buffer is empty
        if (temp_start == temp_end) {
            // check if uart is enabled and data is avaiable
            if (UCSR0B & _BV(RXEN0)) {
                if (UCSR0A & _BV(RXC0)) {
                    result = UDR0;

                    sei();
                    return result;
                }
            }
        }

        // wait for data
        while (temp_start == temp_end) {
            sei();

            // check if uart is still activated
            if ((UCSR0B & _BV(RXEN0)) == 0x00) {
                return 0x00;
            }
            nop();

            cli();
            temp_start = system_uart0_rx_start;
            temp_end   = system_uart0_rx_end  ;
        }

        // load from buffer
        result = system_uart0_rx[temp_start];

        temp_start++;
        if (temp_start >= UART0_RX) {
            temp_start-= UART0_RX;
        }
        system_uart0_rx_start = temp_start;

        sei();
        return result;
    }
#else //#ifdef UART0_RX
    uint8_t uart0_get() {

        uint8_t result;

        cli();

        if ((UCSR0B & _BV(RXEN0)) == 0x00) {
            sei();
            return 0x00;
        }

        while ((UCSR0A & _BV(RXC0)) == 0x00) {
            sei();

            if ((UCSR0B & _BV(RXEN0)) == 0x00) {
                return 0x00;
            }
            nop();

            cli();
        }

        result = UDR0;
        sei();
        return result;
    }
#endif //#ifdef UART0_RX

//**************************[uart0_get_nonblocking]**************************** 29.10.2017
#ifdef UART0_RX
    uint8_t uart0_get_nonblocking() {

        uint8_t mSREG = SREG;
        uint8_t result;

        cli();
        uint8_t temp_start = system_uart0_rx_start;
        uint8_t temp_end   = system_uart0_rx_end  ;

        // check if buffer is not empty
        if (temp_start != temp_end) {
            result = system_uart0_rx[temp_start];

            temp_start++;
            if (temp_start >= UART0_RX) {
                temp_start-= UART0_RX;
            }
            system_uart0_rx_start = temp_start;

            SREG = mSREG;
            return result;
        }

        // check if UART0 is disabled
        if ((UCSR0B & _BV(RXEN0)) == 0x00) {
            SREG = mSREG;
            return 0x00;
        }

        // check if data is in hardware buffer
        if (UCSR0A & _BV(RXC0)) {
            result = UDR0;

            SREG = mSREG;
            return result;
        }

        SREG = mSREG;
        return 0x00;
    }
#else //#ifdef UART0_RX
    uint8_t uart0_get_nonblocking() {

        uint8_t mSREG = SREG;
        uint8_t result;

        cli();

        // check if UART0 is disabled
        if ((UCSR0B & _BV(RXEN0)) == 0x00) {
            SREG = mSREG;
            return 0x00;
        }

        // check if data is in hardware buffer
        if (UCSR0A & _BV(RXC0)) {
            result = UDR0;

            SREG = mSREG;
            return result;
        }

        SREG = mSREG;
        return 0x00;
    }
#endif //#ifdef UART0_RX

//**************************[uart0_txcount_get]******************************** 27.09.2015
#ifdef UART0_TX
    uint8_t uart0_txcount_get() {

        uint8_t mSREG = SREG;
        uint8_t result;

        cli();

        uint8_t temp_start = system_uart0_tx_start;
        uint8_t temp_end   = system_uart0_tx_end  ;

        result = temp_end - temp_start;
        if (temp_end < temp_start) {
            result+= UART0_TX;
        }

        if ((result < 255) && (UCSR0B & _BV(TXEN0))) {
            if ((UCSR0A & _BV(UDRE0)) == 0x00) {
                result++;
            }
        }

        SREG = mSREG;
        return result;
    }
#else //#ifdef UART0_TX
    uint8_t uart0_txcount_get() {

        uint8_t mSREG = SREG;
        uint8_t result;

        cli();

        result = 0x00;

        if (UCSR0B & _BV(TXEN0)) {
            if (UCSR0A & _BV(UDRE0)) {
                result = 1;
            }
        }

        SREG = mSREG;
        return result;
    }
#endif //#ifdef UART0_TX

//**************************[uart0_rxcount_get]******************************** 27.09.2015
#ifdef UART0_RX
    uint8_t uart0_rxcount_get() {

        uint8_t mSREG = SREG;
        uint8_t result;

        cli();

        uint8_t temp_start = system_uart0_rx_start;
        uint8_t temp_end   = system_uart0_rx_end  ;

        result = temp_end - temp_start;
        if (temp_end < temp_start) {
            result+= UART0_RX;
        }

        if ((result < 255) && (UCSR0B & _BV(RXEN0))) {
            if (UCSR0A & _BV(RXC0)) {
                result++;
            }
        }

        SREG = mSREG;
        return result;
    }
#else //#ifdef UART0_RX
    uint8_t uart0_rxcount_get() {

        uint8_t mSREG = SREG;
        uint8_t result;

        cli();

        result = 0x00;

        if (UCSR0B & _BV(RXEN0)) {
            if (UCSR0A & _BV(RXC0)) {
                result = 1;
            }
        }

        SREG = mSREG;
        return result;
    }
#endif //#ifdef UART0_RX

//**************************[uart0_flush]************************************** 27.09.2015
void uart0_flush() {

    uint8_t mSREG = SREG;

    cli();

    #ifdef UART0_TX
        system_uart0_tx_start = 0;
        system_uart0_tx_end   = 0;
    #endif // #ifdef UART0_TX

    #ifdef UART0_RX
        system_uart0_rx_start = 0;
        system_uart0_rx_end   = 0;
    #endif // #ifdef UART0_RX

    SREG = mSREG;
}

//**************************[uart0_baud_set]*********************************** 27.09.2015
uint8_t uart0_baud_set(uint16_t baudrate) {

    uint8_t mSREG = SREG;
    union uint32 temp;

    if (baudrate == 0) {
        return 0x00;
    }

    temp.u = F_CPU / 16;
    temp.u+= baudrate >> 1;
    temp.u/= baudrate;
    temp.u--;

    if (temp.hu) {
        return 0x00;
    }

    if (temp.lu & (~SYSTEM_UART0_UBRR_MASK)) {
        return 0x00;
    }

    cli();
    UBRR0H = temp.lh;
    UBRR0L = temp.ll;
    SREG = mSREG;

    return 0xFF;
}

//**************************[uart0_baud_get]*********************************** 27.09.2015
uint16_t uart0_baud_get(void) {

    uint8_t mSREG = SREG;
    union uint32 temp;
    union uint16 temp_r;

    temp.u = F_CPU / 16;

    cli();
    temp_r.l = UBRR0L;
    temp_r.h = UBRR0H;
    SREG = mSREG;
    temp_r.u&= SYSTEM_UART0_UBRR_MASK;
    temp_r.u++;

    if (temp_r.u) {
        temp.u+= temp_r.u >> 1;
        temp.u/= temp_r.u;
    } else {
        temp.lu = temp.hu;
        temp.hu = 0x0000;
    }

    if (temp.hu) {
        return 0xFFFF;
    }

    return temp.lu;
}

//**************************[uart0_enable]************************************* 27.09.2015
void uart0_enable(void) {

    uint8_t mSREG = SREG;
    cli();

    UCSR0B|= _BV(TXEN0);
    UCSR0B|= _BV(RXEN0);

    #ifdef UART0_TX
        if (system_uart0_tx_end != system_uart0_tx_start) {
            UCSR0B|= _BV(UDRIE0);
        }
    #endif

    #ifdef UART0_RX
        UCSR0B|= _BV(RXCIE0);
    #endif

    SREG = mSREG;
}

//**************************[uart0_disable]************************************ 27.09.2015
void uart0_disable(void) {

    uint8_t mSREG = SREG;
    cli();

    UCSR0B&= ~_BV(TXEN0);
    UCSR0B&= ~_BV(RXEN0);

    UCSR0B&= ~_BV(UDRIE0);
    UCSR0B&= ~_BV(RXCIE0);

    SREG = mSREG;
}

//**************************[uart0_is_enabled]********************************* 27.09.2015
uint8_t uart0_is_enabled(void) {

    if ((UCSR0B & (_BV(RXEN0) | _BV(TXEN0))) == (_BV(RXEN0) | _BV(TXEN0))) {
        return 0xFF;
    } else {
        return 0x00;
    }
}

//**************************[system_uart0_print]******************************* 27.09.2015
void system_uart0_print(void (*out)(uint8_t)) {
    string_from_const(out, "uart0:"                                    "\r\n");

    string_from_const_length(out, "  enabled" , 15);
                                                  string_from_const(out, ": ");
    string_from_bool(out, uart0_is_enabled());  string_from_const(out, "\r\n");

    union uint16 temp;
    uint8_t mSREG = SREG;
    cli();
    temp.l = UBRR0L;
    temp.h = UBRR0H;
    SREG = mSREG;
    temp.u&= SYSTEM_UART0_UBRR_MASK;
    string_from_const_length(out, "  UBRR0", 15); string_from_const(out, ": ");
    string_from_uint(out, temp.u,5);            string_from_const(out, "\r\n");

    string_from_const_length(out, "  baud" , 15); string_from_const(out, ": ");
    string_from_uint(out, uart0_baud_get(),5);  string_from_const(out, "\r\n");

    string_from_const_length(out, "  tx"   , 15); string_from_const(out, ": ");
    string_from_uint(out, uart0_txcount_get(),1);
                                                string_from_const(out, "\r\n");

    string_from_const_length(out, "  rx"   , 15); string_from_const(out, ": ");
    string_from_uint(out, uart0_rxcount_get(),1);
                                                string_from_const(out, "\r\n");

    string_from_const(out,                                             "\r\n");
}

//**************************[system_uart0_print_compiled]********************** 27.09.2015
void system_uart0_print_compiled(void (*out)(uint8_t)) {

    string_from_const(out, SYSTEM_UART0_VERSION                        "\r\n");
    string_from_const(out, SYSTEM_UART0_SUB_VERSION                    "\r\n");

    string_from_const_length(out, "  MCU"  , 15); string_from_const(out, ": ");
    string_from_macro(out, MCU);                string_from_const(out, "\r\n");

    string_from_const_length(out, "  F_CPU", 15); string_from_const(out, ": ");
    string_from_macro(out, F_CPU);              string_from_const(out, "\r\n");

    string_from_const_length(out, "  UART0_RX", 15);
                                                  string_from_const(out, ": ");
    #ifdef UART0_RX
    string_from_macro(out, UART0_RX);           string_from_const(out, "\r\n");
    #else
    string_from_const(out, "[undefined]");      string_from_const(out, "\r\n");
    #endif

    string_from_const_length(out, "  UART0_TX", 15);
                                                  string_from_const(out, ": ");
    #ifdef UART0_TX
    string_from_macro(out, UART0_TX);           string_from_const(out, "\r\n");
    #else
    string_from_const(out, "[undefined]");      string_from_const(out, "\r\n");
    #endif

    string_from_const(out,                                             "\r\n");
}

//**************************[system_uart0_init]********************************
// siehe controllerspezifische Datei

//**************************[ISR(USART0_UDRE_vect)]****************************  27.09.2015
#ifdef UART0_TX
    ISR(USART0_UDRE_vect) {

        uint8_t temp_start = system_uart0_tx_start;
        uint8_t temp_end   = system_uart0_tx_end  ;

        // check if buffer is empty
        if (temp_start == temp_end) {
            UCSR0B&= ~_BV(UDRIE0);
            return;
        }

        // send data
        UDR0 = system_uart0_tx[temp_start];

        // calculate next position
        temp_start++;
        if (temp_start >= UART0_TX) {
            temp_start-= UART0_TX;
        }
        system_uart0_tx_start = temp_start;

        // check if buffer is empty
        if (temp_start == temp_end) {
            UCSR0B&= ~_BV(UDRIE0);
        }
    }
#endif //#ifdef UART0_TX

//**************************[ISR(USART0_RX_vect)]****************************** 27.09.2015
#ifdef UART0_RX
    ISR(USART0_RX_vect) {

        uint8_t temp_start = system_uart0_rx_start;
        uint8_t temp_end   = system_uart0_rx_end  ;

        // save data
        system_uart0_rx[temp_end] = UDR0;

        // calculate next position
        temp_end++;
        if (temp_end >= UART0_RX) {
            temp_end-= UART0_RX;
        }

        // check if buffer is not full
        if (temp_start != temp_end) {
            // save data in buffer
            system_uart0_rx_end = temp_end;
        }
    }
#endif //#ifdef UART0_RX
