/******************************************************************************
* source/uart1/uart1.c                                                        *
* ====================                                                        *
*                                                                             *
* Author : Peter Weissig                                                      *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/peterweissig/robolib                                   *
******************************************************************************/

//**************************<File version>*************************************
#define SYSTEM_UART1_VERSION \
    "source/uart1/uart1.c 27.09.2015 V1.0.0"

//**************************<Included files>***********************************
#include "system/uart1.h"

#include "system/assembler.h"
#include "system/string.h"
#include "system/types.h"

#include <avr/io.h>

//**************************<Variables>****************************************
#ifdef UART1_TX
    volatile uint8_t system_uart1_tx[UART1_TX];
    volatile uint8_t system_uart1_tx_start;
    volatile uint8_t system_uart1_tx_end;
#endif //#ifdef UART1_TX

#ifdef UART1_RX
    volatile uint8_t system_uart1_rx[UART1_RX];
    volatile uint8_t system_uart1_rx_start;
    volatile uint8_t system_uart1_rx_end;
#endif //#ifdef UART1_RX

//**************************<Prototypes>***************************************

//**************************<Files>********************************************

#if    defined (__AVR_ATmega2561__) // switch micro controller
    #include <source/uart1/uart1_atmega2561.c>
    #define MCU atmega2561
#elif  defined (__AVR_ATmega64__  ) // switch micro controller
    #include <source/uart1/uart1_atmega64.c>
    #define MCU atmega64
#else                               // switch micro controller
    #error "device is not supported (yet)"
#endif                              // switch micro controller

//**************************[uart1_send]*************************************** 27.09.2015
void uart1_send(uint8_t data) {

    #ifndef UART1_TX
        if ((UCSR1B & _BV(TXEN1)) == 0x00) {
            sei();
            return;
        }
    #endif // #ifndef UART1_TX

    while (uart1_send_nonblocking(data) == 0x00) {
        sei();
        if ((UCSR1B & _BV(TXEN1)) == 0x00) {
            return;
        }
    }
}

//**************************[uart1_send_nonblocking]*************************** 27.09.2015
#ifdef UART1_TX
    uint8_t uart1_send_nonblocking(uint8_t data) {

        uint8_t mSREG = SREG;

        cli();
        uint8_t temp_start = system_uart1_tx_start;
        uint8_t temp_end   = system_uart1_tx_end  ;

        // check if buffer is empty
        if (temp_start == temp_end) {
            if (UCSR1B & _BV(TXEN1)) {
                if (UCSR1A & _BV(UDRE1)) {
                    UDR1 = data;

                    SREG = mSREG;
                    return 0xFF;
                }
            }
        }

        // save data in buffer
        system_uart1_tx[temp_end] = data;

        // calculate next position
        temp_end++;
        if (temp_end >= UART1_TX) {
            temp_end-= UART1_TX;
        }

        // check if buffer is full
        if (temp_start == temp_end) {
            SREG = mSREG;
            return 0x00;
        }

        // save new buffer size
        system_uart1_tx_end = temp_end;

        // activate interrupt for transmission
        if (UCSR1B & _BV(TXEN1)) {
            UCSR1B|= _BV(UDRIE1);
        }

        SREG = mSREG;
        return 0xFF;
    }
#else //#ifdef UART1_TX
    uint8_t uart1_send_nonblocking(uint8_t data) {

        uint8_t mSREG = SREG;

        cli();

        if ((UCSR1B & _BV(TXEN1)) == 0x00) {
            SREG = mSREG;
            return 0x00;
        }

        if (UCSR1A & _BV(UDRE1)) {
            UDR1 = data;

            SREG = mSREG;
            return 0xFF;
        }

        SREG = mSREG;
        return 0x00;
    }
#endif //#ifdef UART1_TX

//**************************[uart1_get]**************************************** 27.09.2015
#ifdef UART1_RX
    uint8_t uart1_get() {

        uint8_t result;

        cli();
        uint8_t temp_start = system_uart1_rx_start;
        uint8_t temp_end   = system_uart1_rx_end  ;

        // check if buffer is empty
        if (temp_start == temp_end) {
            if (UCSR1B & _BV(RXEN1)) {
                if (UCSR1A & _BV(RXC1)) {
                    result = UDR1;

                    sei();
                    return result;
                }
            }
        }

        while (temp_start == temp_end) {
            sei();

            if ((UCSR1B & _BV(RXEN1)) == 0x00) {
                return 0x00;
            }
            nop();

            cli();
        }

        result = system_uart1_rx[temp_start];

        temp_start++;
        if (temp_start >= UART1_RX) {
            temp_start-= UART1_RX;
        }
        system_uart1_rx_start = temp_start;

        sei();
        return result;
    }
#else //#ifdef UART1_RX
    uint8_t uart1_get(uint8_t data) {

        uint8_t result;

        cli();

        if ((UCSR1B & _BV(RXEN1)) == 0x00) {
            sei();
            return 0x00;
        }

        while ((UCSR1A & _BV(RXC1)) == 0x00) {
            sei();

            if ((UCSR1B & _BV(RXEN1)) == 0x00) {
                return 0x00;
            }
            nop();

            cli();
        }

        result = UDR1;
        sei();
        return result;
    }
#endif //#ifdef UART1_RX

//**************************[uart1_get_nonblocking]**************************** 27.09.2015
#ifdef UART1_RX
    uint8_t uart1_get_nonblocking() {

        uint8_t mSREG = SREG;
        uint8_t result;

        cli();
        uint8_t temp_start = system_uart1_rx_start;
        uint8_t temp_end   = system_uart1_rx_end  ;

        // check if buffer is not empty
        if (temp_start != temp_end) {
            result = system_uart1_rx[temp_start];

            temp_start++;
            if (temp_start >= UART1_RX) {
                temp_start-= UART1_RX;
            }
            system_uart1_rx_start = temp_start;

            SREG = mSREG;
            return result;
        }

        // check if UART1 is disabled
        if ((UCSR1B & _BV(RXEN1)) == 0x00) {
            SREG = mSREG;
            return 0x00;
        }

        // check if data is in hardware buffer
        if (UCSR1A & _BV(RXC1)) {
            result = UDR1;

            SREG = mSREG;
            return result;
        }

        SREG = mSREG;
        return 0x00;
    }
#else //#ifdef UART1_RX
    uint8_t uart1_get_nonblocking(uint8_t data) {

        uint8_t mSREG = SREG;
        uint8_t result;

        cli();

        // check if UART1 is disabled
        if ((UCSR1B & _BV(RXEN1)) == 0x00) {
            SREG = mSREG;
            return 0x00;
        }

        // check if data is in hardware buffer
        if (UCSR1A & _BV(RXC1)) {
            result = UDR1;

            SREG = mSREG;
            return result;
        }

        SREG = mSREG;
        return 0x00;
    }
#endif //#ifdef UART1_RX

//**************************[uart1_txcount_get]******************************** 27.09.2015
#ifdef UART1_TX
    uint8_t uart1_txcount_get() {

        uint8_t mSREG = SREG;
        uint8_t result;

        cli();

        uint8_t temp_start = system_uart1_tx_start;
        uint8_t temp_end   = system_uart1_tx_end  ;

        result = temp_end - temp_start;
        if (temp_end < temp_start) {
            result+= UART1_TX;
        }

        if ((result < 255) && (UCSR1B & _BV(TXEN1))) {
            if ((UCSR1A & _BV(UDRE1)) == 0x00) {
                result++;
            }
        }

        SREG = mSREG;
        return result;
    }
#else //#ifdef UART1_TX
    uint8_t uart1_txcount_get() {

        uint8_t mSREG = SREG;
        uint8_t result;

        cli();

        result = 0x00;

        if (UCSR1B & _BV(TXEN1)) {
            if (UCSR1A & _BV(UDRE1)) {
                result = 1;
            }
        }

        SREG = mSREG;
        return result;
    }
#endif //#ifdef UART1_TX

//**************************[uart1_rxcount_get]******************************** 27.09.2015
#ifdef UART1_RX
    uint8_t uart1_rxcount_get() {

        uint8_t mSREG = SREG;
        uint8_t result;

        cli();

        uint8_t temp_start = system_uart1_rx_start;
        uint8_t temp_end   = system_uart1_rx_end  ;

        result = temp_end - temp_start;
        if (temp_end < temp_start) {
            result+= UART1_RX;
        }

        if ((result < 255) && (UCSR1B & _BV(RXEN1))) {
            if (UCSR1A & _BV(RXC1)) {
                result++;
            }
        }

        SREG = mSREG;
        return result;
    }
#else //#ifdef UART1_RX
    uint8_t uart1_rxcount_get() {

        uint8_t mSREG = SREG;
        uint8_t result;

        cli();

        result = 0x00;

        if (UCSR1B & _BV(RXEN1)) {
            if (UCSR1A & _BV(RXC1)) {
                result = 1;
            }
        }

        SREG = mSREG;
        return result;
    }
#endif //#ifdef UART1_RX

//**************************[uart1_flush]************************************** 27.09.2015
void uart1_flush() {

    uint8_t mSREG = SREG;

    cli();

    #ifdef UART1_TX
        system_uart1_tx_start = 0;
        system_uart1_tx_end   = 0;
    #endif // #ifdef UART1_TX

    #ifdef UART1_RX
        system_uart1_rx_start = 0;
        system_uart1_rx_end   = 0;
    #endif // #ifdef UART1_RX

    SREG = mSREG;
}

//**************************[uart1_baud_set]*********************************** 27.09.2015
uint8_t uart1_baud_set(uint16_t baudrate) {

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

    if (temp.lu & (~SYSTEM_UART1_UBRR_MASK)) {
        return 0x00;
    }

    cli();
    UBRR1H = temp.lh;
    UBRR1L = temp.ll;
    SREG = mSREG;

    return 0xFF;
}

//**************************[uart1_baud_get]*********************************** 27.09.2015
uint16_t uart1_baud_get(void) {

    uint8_t mSREG = SREG;
    union uint32 temp;
    union uint16 temp_r;

    temp.u = F_CPU / 16;

    cli();
    temp_r.l = UBRR1L;
    temp_r.h = UBRR1H;
    SREG = mSREG;
    temp_r.u&= SYSTEM_UART1_UBRR_MASK;
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

//**************************[uart1_enable]************************************* 27.09.2015
void uart1_enable(void) {

    uint8_t mSREG = SREG;
    cli();

    UCSR1B|= _BV(TXEN1);
    UCSR1B|= _BV(RXEN1);

    #ifdef UART1_TX
        if (system_uart1_tx_end != system_uart1_tx_start) {
            UCSR1B|= _BV(UDRIE1);
        }
    #endif

    #ifdef UART1_RX
        UCSR1B|= _BV(RXCIE1);
    #endif

    SREG = mSREG;
}

//**************************[uart1_disable]************************************ 27.09.2015
void uart1_disable(void) {

    uint8_t mSREG = SREG;
    cli();

    UCSR1B&= ~_BV(TXEN1);
    UCSR1B&= ~_BV(RXEN1);

    UCSR1B&= ~_BV(UDRIE1);
    UCSR1B&= ~_BV(RXCIE1);

    SREG = mSREG;
}

//**************************[uart1_is_enabled]********************************* 27.09.2015
uint8_t uart1_is_enabled(void) {

    if ((UCSR1B & (_BV(RXEN1) | _BV(TXEN1))) == (_BV(RXEN1) | _BV(TXEN1))) {
        return 0xFF;
    } else {
        return 0x00;
    }
}

//**************************[system_uart1_print]******************************* 27.09.2015
void system_uart1_print(void (*out)(uint8_t)) {

    string_from_const(out, "uart1:"                                    "\r\n");

    string_from_const_length(out, "  enabled", 15);
                                                  string_from_const(out, ": ");
    string_from_bool(out, uart1_is_enabled());  string_from_const(out, "\r\n");

    union uint16 temp;
    uint8_t mSREG = SREG;
    cli();
    temp.l = UBRR1L;
    temp.h = UBRR1H;
    SREG = mSREG;
    temp.u&= SYSTEM_UART1_UBRR_MASK;
    string_from_const_length(out, "  UBRR1", 15); string_from_const(out, ": ");
    string_from_uint(out, temp.u,5);            string_from_const(out, "\r\n");

    string_from_const_length(out, "  baud" , 15); string_from_const(out, ": ");
    string_from_uint(out, uart1_baud_get(),5);  string_from_const(out, "\r\n");

    string_from_const_length(out, "  tx"   , 15); string_from_const(out, ": ");
    string_from_uint(out, uart1_txcount_get(),1);
                                                string_from_const(out, "\r\n");

    string_from_const_length(out, "  rx"   , 15); string_from_const(out, ": ");
    string_from_uint(out, uart1_rxcount_get(),1);
                                                string_from_const(out, "\r\n");

    string_from_const(out,                                             "\r\n");
}

//**************************[system_uart1_print_compiled]********************** 27.09.2015
void system_uart1_print_compiled(void (*out)(uint8_t)) {

    string_from_const(out, SYSTEM_UART1_VERSION                        "\r\n");
    string_from_const(out, SYSTEM_UART1_SUB_VERSION                    "\r\n");

    string_from_const_length(out, "  MCU"  , 15); string_from_const(out, ": ");
    string_from_macro(out, MCU);                string_from_const(out, "\r\n");

    string_from_const_length(out, "  F_CPU", 15); string_from_const(out, ": ");
    string_from_macro(out, F_CPU);              string_from_const(out, "\r\n");

    string_from_const_length(out, "  UART1_RX", 15);
                                                  string_from_const(out, ": ");
    #ifdef UART1_RX
        string_from_macro(out, UART1_RX);       string_from_const(out, "\r\n");
    #else
        string_from_const(out, "[undefined]");  string_from_const(out, "\r\n");
    #endif

    string_from_const_length(out, "  UART1_TX", 15);
                                                  string_from_const(out, ": ");
    #ifdef UART1_TX
        string_from_macro(out, UART1_TX);       string_from_const(out, "\r\n");
    #else
        string_from_const(out, "[undefined]");  string_from_const(out, "\r\n");
    #endif

    string_from_const(out,                                             "\r\n");
}

//**************************[system_uart1_init]********************************
// siehe controllerspezifische Datei

//**************************[ISR(USART1_UDRE_vect)]**************************** 27.09.2015
#ifdef UART1_TX
    ISR(USART1_UDRE_vect) {

        uint8_t temp_start = system_uart1_tx_start;
        uint8_t temp_end   = system_uart1_tx_end  ;

        // check if buffer is empty
        if (temp_start == temp_end) {
            UCSR1B&= ~_BV(UDRIE1);
            return;
        }

        // send data
        UDR1 = system_uart1_tx[temp_start];

        // calculate next position
        temp_start++;
        if (temp_start >= UART1_TX) {
            temp_start-= UART1_TX;
        }
        system_uart1_tx_start = temp_start;

        // check if buffer is empty
        if (temp_start == temp_end) {
            UCSR1B&= ~_BV(UDRIE1);
        }
    }
#endif //#ifdef UART1_TX

//**************************[ISR(USART1_RX_vect)]****************************** 27.09.2015
#ifdef UART1_RX
    ISR(USART1_RX_vect) {

        uint8_t temp_start = system_uart1_rx_start;
        uint8_t temp_end   = system_uart1_rx_end  ;

        // save data
        system_uart1_rx[temp_end] = UDR1;

        // calculate next position
        temp_end++;
        if (temp_end >= UART1_RX) {
            temp_end-= UART1_RX;
        }

        // check if buffer is not full
        if (temp_start != temp_end) {
            // save data in buffer
            system_uart1_rx_end = temp_end;
        }
    }
#endif //#ifdef UART1_RX
