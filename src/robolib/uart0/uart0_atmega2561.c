/******************************************************************************
* uart0/uart0_atmega2561.c                                                    *
* ========================                                                    *
*                                                                             *
* Author : Peter Weissig                                                      *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/RoboAG/avr_robolib                                     *
******************************************************************************/

//**************************<File version>*************************************
#define ROBOLIB_UART0_SUB_VERSION \
  "robolib/uart0/uart0_atmega2561.c 27.09.2015 V1.0.0"

#define ROBOLIB_UART0_UBRR_MASK 0x0FFF

//**************************[robolib_uart0_init]******************************** 27.09.2015
void robolib_uart0_init() {
    #ifdef UART0_TX
        robolib_uart0_tx_start = 0;
        robolib_uart0_tx_end   = 0;
    #endif // #ifdef UART0_TX

    #ifdef UART0_RX
        robolib_uart0_rx_start = 0;
        robolib_uart0_rx_end   = 0;
    #endif // #ifdef UART0_RX

    // set hardware registers
    UCSR0A = 0x00;
        // Bit 7   (RXC0  ) =    0 interrupt flag (receive  complete)
        // Bit 6   (TXC0  ) =    0 interrupt flag (transmit complete)
        // Bit 5   (UDRE0 ) =    0 interrupt flag (data ... empty)
        // Bit 4   (FE0   ) =    0 flag (frame error)
        // Bit 3   (DOR0  ) =    0 flag (data overrun)
        // Bit 2   (UPE0  ) =    0 flag (parity error)
        // Bit 1   (U2X0  ) =    0 normal speed (prescaler = 16)
        // Bit 0   (MPCM0 ) =    0 normal speed (no multi-processor mode)

    UCSR0B = 0x00;
        // Bit 7   (RXCIE0) =    0 interrupt enable (receive  complete)
        // Bit 6   (TXCIE0) =    0 interrupt enable (transmit complete)
        // Bit 5   (UDRIE0) =    0 interrupt enable (data ... empty)
        // Bit 4   (RXEN0 ) =    0 receiver disabled
        // Bit 3   (TXEN0 ) =    0 transmitter disabled
        // Bit 2   (UCSZ02) =    0 character size (8 bit) - see also UCSR0C
        // Bit 1   (RXB80 ) =    0 rx bit 8 (not used)
        // Bit 0   (TXB80 ) =    0 tx bit 8 (not used)

    UCSR0C = _BV(USBS0) | _BV(UCSZ01) | _BV(UCSZ00);
        // Bit 6   (UMSEL0) =    0 asynchronous operation
        // Bit 4-5 (UPM0n ) =   00 parity check disabled
        // Bit 3   (USBS0 ) =    1 two stop bits
        // Bit 1-2 (UCSZ0n) =  011 character size (8 bit) - see also UCSR0B
        // Bit 0   (UCPOL0) =    0 clock polarity (not used)

    // set initial baudrate
    uart0_baud_set(UART0_BAUD);

    // start uart0
    uart0_enable();
}
