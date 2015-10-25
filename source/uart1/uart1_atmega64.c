/******************************************************************************
* source/uart1/uart1_atmega64.c                                               *
* =============================                                               *
*                                                                             *
* Author : Peter Weissig                                                      *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/peterweissig/robolib                                   *
******************************************************************************/

//**************************<File version>*************************************
#define SYSTEM_UART1_SUB_VERSION \
    "source/uart1/uart1_atmega64.c 27.09.2015 V1.0.0"

#define SYSTEM_UART1_UBRR_MASK 0x0FFF

//**************************[system_uart1_init]******************************** 27.09.2015
void system_uart1_init() {
    #ifdef UART1_TX
        system_uart1_tx_start = 0;
        system_uart1_tx_end   = 0;
    #endif // #ifdef UART1_TX

    #ifdef UART1_RX
        system_uart1_rx_start = 0;
        system_uart1_rx_end   = 0;
    #endif // #ifdef UART1_RX

    // set hardware registers
    UCSR1A = 0x00;
        // Bit 7   (RXC1  ) =    0 interrupt flag (receive  complete)
        // Bit 6   (TXC1  ) =    0 interrupt flag (transmit complete)
        // Bit 5   (UDRE1 ) =    0 interrupt flag (data ... empty)
        // Bit 4   (FE1   ) =    0 flag (frame error)
        // Bit 3   (DOR1  ) =    0 flag (data overrun)
        // Bit 2   (UPE1  ) =    0 flag (parity error)
        // Bit 1   (U2X1  ) =    0 normal speed (prescaler = 16)
        // Bit 0   (MPCM1 ) =    0 normal speed (no multi-processor mode)

    UCSR1B = 0x00;
        // Bit 7   (RXCIE1) =    0 interrupt enable (receive  complete)
        // Bit 6   (TXCIE1) =    0 interrupt enable (transmit complete)
        // Bit 5   (UDRIE1) =    0 interrupt enable (data ... empty)
        // Bit 4   (RXEN1 ) =    0 receiver disabled
        // Bit 3   (TXEN1 ) =    0 transmitter disabled
        // Bit 2   (UCSZ12) =    0 character size (8 bit) - see also UCSR0C
        // Bit 1   (RXB81 ) =    0 rx bit 8 (not used)
        // Bit 0   (TXB81 ) =    0 tx bit 8 (not used)

    UCSR1C = _BV(USBS1) | _BV(UCSZ11) | _BV(UCSZ10);
        // Bit 6   (UMSEL1) =    0 asynchronous operation
        // Bit 4-5 (UPM1n ) =   00 parity check disabled
        // Bit 3   (USBS1 ) =    1 two stop bits
        // Bit 1-2 (UCSZ1n) =  011 character size (8 bit) - see also UCSR0B
        // Bit 0   (UCPOL1) =    0 clock polarity (not used)

    // set initial baudrate
    uart1_baud_set(UART1_BAUD);

    // start uart1
    uart1_enable();
}
